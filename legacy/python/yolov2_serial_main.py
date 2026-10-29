import threading
import cv2
import numpy as np
import time
import serial

# 아두이노의 포트 및 통신속도 설정
portName = "/dev/cu.usbserial-130"
arduino = 0
arduino = serial.Serial(portName, 9600)

#카메라 설정
device = 0
cap = cv2.VideoCapture(device)

# object detection 설정
whT = 320
confThreshold_1 = 0.5
confThreshold_2 = 0.7
nmsThreshold = 0.3

# 스레드에서 object detection을 한 결과를 저장하는 전역변수들
global indices
global bbox
global classIds
global confs
indices = []
bbox = []
classIds = []
confs = []

# object detection할 수 있는 목록이 저장된 파일에서 목록 불러오기
classesFile = 'yolo.names'
with open(classesFile, 'rt') as f:
	classNames = f.read().split('\n')

# object detection하는 모델 불러오기
model_config = 'yolov2-tiny.cfg'
model_weights = 'yolov2-tiny.weights'
net = cv2.dnn.readNetFromDarknet(model_config, model_weights)

# object detection 설정 2
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)

# objectDetect 스레드가 실행 중인지 상태를 저장하는 전역변수
global isObjectDetectRunning
isObjectDetectRunning = False

# arduinoRead 스레드가 실행 중인지 상태를 저장하는 전역변수
global isArduinoWritng
isArduinoWritng = False

global isBirdDetected
isBirdDetected = False

# object detection하는 함수
def findObjects(frame):
    startTime = time.time()
    blob = cv2.dnn.blobFromImage(frame, 1/255, (whT, whT), [0,0,0], True, crop=False)
    net.setInput(blob)
    
    layerNames = net.getLayerNames()
    outputNames = [layerNames[i - 1] for i in net.getUnconnectedOutLayers()]
    outputs = net.forward(outputNames)
    hT, wT, cT = frame.shape
    bbox_ = []
    classIds_ = []
    confs_ = []
    
    global isBirdDetected
    isBirdDetected = False
    for output in outputs:
        for det in output:
            scores = det[5:]
            classId = np.argmax(scores)
            confidence = scores[classId]
            if classId == 0 and confidence > confThreshold_1:
                isBirdDetected = True
                w, h = int(det[2]*wT), int(det[3]*hT)
                x, y = int((det[0]*wT) - w/2), int((det[1]*hT) - h/2)
                bbox_.append([x,y,w,h])
                classIds_.append(classId)
                confs_.append(float(confidence))
                
    indices_ = cv2.dnn.NMSBoxes(bbox_, confs_, confThreshold_2, nmsThreshold)
    
    # 결과 전역변수에 저장
    # (굳이 지역변수를 만들어서 전역변수에 값을 바꿔주는 이유는 object detection하는 동안 값이 바뀌게 되는데
    #  계속해서 반복하는 makeRect를 할 때 값이 도중에 변경되어서 index out of range를 막기 위함)
    global bbox
    global classIds
    global confs
    global indices
    bbox = bbox_
    classIds = classIds_
    confs = confs_
    indices =indices_
    print(f"식별된 대상 : {len(indices)} 개")
    print(f"time : {time.time() - startTime}")

    # 끝났다는 것을 전역변수에 저장해서 알림
    global isObjectDetectRunning
    isObjectDetectRunning = False


# object detection한 결과를 토대로 사각형과 문구를 표시
def makeRect(frame):
    global indices
    global bbox
    global classIds
    global confs
    indices_ = indices
    bbox_ = bbox
    classIds_ = classIds
    confs_ = confs

    for i in indices_:
        box = bbox_[i]
        x, y, w, h = box[0], box[1], box[2], box[3]
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0, 255, 0), 2)
        cv2.putText(frame, f"{classNames[classIds_[i]].upper()} {int(confs_[i]*100)}%", (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

# 아두이노로부터 데이터 수신하는 함수
def arduinoWrite():
    global isBirdDetected
    if isBirdDetected:
        arduino.write("detected".encode())
    else:
        arduino.write("not-detected".encode())
    time.sleep(0.2)
    global isArduinoWritng
    isArduinoWritng = False

while True:
    # 현재 프레임 캡쳐
    success, frame = cap.read()

    # 만약 프레임이 캡쳐가 되지 않는다면 종료
    if not success:
        break
    
    # objectDetect 스레드 생성
    objectDetect = threading.Thread(target=findObjects, args=(frame,))

    # objectDetect 스레드가 실행 중이 아니라면 실행
    if not isObjectDetectRunning:
        isObjectDetectRunning = True
        objectDetect.start()

    # object detection 결과 표시
    makeRect(frame)
    
    # 결과와 현재 프레임 표시
    cv2.imshow("Image", frame)

    # arduinoRead 스레드 생성
    arduinoWriteT = threading.Thread(target=arduinoWrite)

    # arduinoRead 스레드가 실행 중이 아니라면 실행
    if not isArduinoWritng:
        isArduinoWritng = True
        arduinoWriteT.start()

    # 만약 esc를 누르면 프로그램 종료
    key = cv2.waitKey(1)
    if key == 27:
        exit()

import cv2
import numpy as np

device = '1523.jpg'#device의 값을 1523.jpg로 정하기
cap = cv2.VideoCapture(device)#cap의 값을 cv2에서 캡처 된 값을 device로 열기
whT = 320#whT의 값을 320으로 정하기

confThreshold_1 = 0.5#confThreshold_1의 값을 0.5로 정하기
confThreshold_2 = 0.7#confThreshold_2의 값을 0.7로 정하기
nmsThreshold = 0.3#nmsThreshold의 값을 0.5로 정하기

classesFile = 'obj.names'#classesFile의 값을 obj.names로 정하기

with open(classesFile, 'rt') as f:#classesFile과 rt를 f로 지정하기
	classNames = f.read().split('\n')#f의 값을 읽고 \n이 나올 때 마다 문자열로 나누기

model_config = 'yolov3-tiny.cfg'#model_config의 값을 yolov3-tiny.cfg로 정하기
model_weights = 'yolov3-tiny_best.weights'#model_weights의 값을 yolo-tiny_best,weights로 정하기

net = cv2.dnn.readNetFromDarknet(model_config, model_weights)#다크넷에서 model_config와 model_weights의 값을 읽고 net의 값을로 정하기
net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)#
net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)#

def findObjects(outputs, img):#findObjects라는 함수에서 outputs,img의 값을 찾기
    hT, wT, cT = img.shape#img를 튜플로 변환시키시
    bbox = []#bbox를 리스트에 넣기
    classIds = []#classlds를 리스트에 넣기
    confs = []#confs를 리스트에 넣기
    
    for output in outputs:#output로 outputs의 값을 순서대로 넣어주기
        for det in output:#det으로 output의 값을 순서대로 넣어주기 
            scores = det[5:]#scores의 값을 det의 값을 리스트의 5~끝까지로 정하기
            classId = np.argmax(scores)#scores의 값중 가장 큰 값을 찾고 classld의 값으로 정하기
            confidence = scores[classId]#confidence의 값을 scores에 classld로 정하기
            if confidence > confThreshold_1:#만약 confidence의 값이 confThreshold_l보다 크다면
                w, h = int(det[2]*wT), int(det[3]*hT)#det[2]*wT와 int(det[3]*hT)를 정수로 바꾸기
                x, y = int((det[0]*wT) - w/2), int((det[1]*hT) - h/2)#det[0]*wT) - w/2와 det[1]*hT를 정수로 바꾸기
                bbox.append([x,y,w,h])#x,y,w,h의 값을 객체로 bbox에 저장하기
                classIds.append(classId)#classld의 값을 classlds에 객체로 저장하기
                confs.append(float(confidence))#float(cinfidence)의 값을 confs에 객체로 저장하기
          
    indices = cv2.dnn.NMSBoxes(bbox, confs, confThreshold_2, nmsThreshold)#
    print(f"식별된 대상 : {len(indices)} 개")#f"식별된 대상 : {len(indices)} 개"
    
    for i in indices:#i에 indices의 값을 순서대로 넣기
        i = i#i의 값을 i로 정하기
        box = bbox[i]#box의 값을 bbox의 i번째로 정하기
        x, y, w, h = box[0], box[1], box[2], box[3]#x,y,w,h의 값을 box리스트의 0,1,2,3번째로 정하기
        cv2.rectangle(img, (x,y), (x+w, y+h), (0, 255, 0), 2)#cv2에서 img,(x,y),(x+w,y+h),(0,255,0),2에 직사각형을 만들기
        cv2.putText(img, f"{classNames[classIds[i]].upper()} {int(confs[i]*100)}%",#img, f"{classNames[classIds[i]].upper()} {int(confs[i]*100)}%"와(x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)의 값을 cv2에 넣기 
        			(x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            
while True:#값이 true 일 때
    success, img = cap.read()#cap의 값을 읽고 img의 값으로 정하기    
    
    if not success:#만약 값이 success가 아니라면
        break#반복중지    
    blob = cv2.dnn.blobFromImage(img, 1/255, (whT, whT), [0,0,0], True, crop=False)#blob에 바이너리 형태로 이미지 객체 저장하기
    net.setInput(blob)#net에 blob의 값 넣기
    
    layerNames = net.getLayerNames()#layernames의 값을 net.layernames()로 정하기
    outputNames = [layerNames[i - 1] for i in net.getUnconnectedOutLayers()]#outputNames의 값을 layerNames[i - 1] for i in net.getUnconnectedOutLayers()로 정하기
    outputs = net.forward(outputNames)#outputs의 값을 net.forward(outputNames)으로 정하기
    
    findObjects(outputs, img)#outputs와 img를 찾기

    img = cv2.resize(img,(1600,880))#cv2에서 img를 1600,8800사이즈로 리사이즈하기
    
    cv2.imshow("Image", img)#cv2에서 image와 img를 색칠하기
    key = cv2.waitKey(1)#1초 기다리기
    if key == 27:#만약 key가 27이라면
    	break#반복중



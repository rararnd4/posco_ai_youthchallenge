# 코드 및 자료 목록

## 핵심 코드

| 파일 | 역할 | 상태 |
|---|---|---|
| `src/raspberry_pi/bird_detection_and_servo.py` | 카메라 프레임 수집, YOLO 추론, NMS, 결과 표시, 서보 제어 | 저장소 구조에 맞게 경로·서보 조건 정리 |
| `src/arduino/bird_deterrent/bird_deterrent.ino` | PIR·초음파 조건과 시리얼 탐지 신호 처리, DFPlayer 음원 재생 | 최종 폴더에서 확인 |
| `model/yolov7-tiny.cfg` | 1개 클래스용 Darknet 형식 YOLOv7-tiny 설정 | `max_batches=6000`, 입력 416×416 |
| `model/yolov7-tiny_best.weights` | 학습 가중치 | 약 24.1MB |

## 개발 과정 코드

- `legacy/python/yolov2_serial_main.py`: YOLOv2-tiny와 Arduino 시리얼 통신을 연결한 초기 통합 코드
- `legacy/python/yolov3_tiny_commented.py`: 단일 이미지 기반 YOLOv3-tiny 탐지 코드와 학습용 주석
- `legacy/python/servo_test.py`: 서보 모터 왕복 시험
- `legacy/python/serial_port_search.py`: 연결 가능한 시리얼 포트 확인
- `legacy/arduino/bdsv_arduino_1.ino`: 초기 센서·스피커 통합 시도
- `legacy/arduino/bdsv_arduino_2.ino`: PIR·초음파·버튼·시리얼 신호 조건 정리
- `legacy/arduino/bdsv_arduino_3_draft.ino`: DFPlayer Mini 기반 음원 재생으로 변경한 버전
- 나머지 Arduino 파일: PIR, 수위 센서, LCD, 부저, 스피커, DFPlayer 단위 시험 코드

## 시각 자료

- `assets/training_charts/yolov3-tiny.png`: 2,000회 학습 기록
- `assets/training_charts/yolov7-tiny.png`: 6,000회 학습 기록
- `assets/detection_results/yolov7-tiny-indoor.JPG`: 실내 조류 탐지 결과
- `docs/diagrams/시스템구조도.PNG`: 카메라·서보·센서·스피커 연결 개념
- `docs/diagrams/시스템흐름도.PNG`: Raspberry Pi와 Arduino 처리 흐름
- `docs/diagrams/라즈베리파이 회로도.png`: Raspberry Pi 배선
- `docs/diagrams/아두이노 회로도.png`: Arduino 배선

## Drive에서 확인한 관련 자료

Drive에는 코드 외에도 `본선제출`, `멘토링`, `포스코 ICT AI 대회(2022)_예선`, 데이터·사진 폴더가 있습니다. 이번 정리본에는 코드와 직접 연결되는 시스템 흐름도·회로도와 사용자가 제공한 학습 결과 이미지를 먼저 포함했습니다. 대용량 원본 데이터와 영상은 Drive에 그대로 보존되어 있습니다.

추가 확인 결과 `배경이 시연장인 새 학습` 폴더에는 다음 학습 재현 파일이 있습니다.

- `obj.data`, `obj.names`
- `train.txt`, `test.txt`
- `yolov3-tiny.cfg`, `yolov3-tiny_best.weights`
- `yolov7-tiny.cfg`, `yolov7-tiny_best.weights`, `yolov7-tiny.conv.87`
- `make_img.py`, `test_train.py`, `test.zip`
- YOLOv3·v7 학습 그래프와 탐지 결과 이미지·영상

`테스트자료/새 인식 테스트` 폴더는 현재 비어 있습니다. `본선제출/모델링`에는 성능표가 아니라 프로토타입 3D 모델 이미지 5장이 들어 있습니다.


import cv2
import mediapipe as mp
import numpy as np
import time
import math
import face_recognition
import os
import paho.mqtt.client as mqtt
from pyzbar.pyzbar import decode
import json

def decoder(image):
    gray_img = cv2.cvtColor(image,0)
    barcode = decode(gray_img)

    for obj in barcode:
        points = obj.polygon
        (x,y,w,h) = obj.rect
        pts = np.array(points, np.int32)
        pts = pts.reshape((-1, 1, 2))
        cv2.polylines(image, [pts], True, (0, 255, 0), 3)

        barcodeData = obj.data.decode("utf-8")
        barcodeType = obj.type
        string = "Data: " + str(barcodeData) + " | Type: " + str(barcodeType)
        
        cv2.putText(image, string, (x,y), cv2.FONT_HERSHEY_SIMPLEX,0.8,(0,0,255), 2)
        # print("Barcode: "+barcodeData +" | Type: "+barcodeType)
        return barcodeData

def passCheck(finger, dummyC, dummyL, passA):
    # handL = []
    # handR = []
    # for i, j in enumerate(finger):
    #     if 'L' in j:
    #         handL.append(j)
    #     else:
    #         handR.append(j)
    dummyC.append(len(finger))
    for i in range(len(passA)):
        if (([0, 0, 0] == dummyL[i]).all() and passA[i][0] in dummyC):
            dummyL[i] = [1, 0, 0]
            dummyC.clear()
        elif (([1, 0, 0] == dummyL[i]).all() and passA[i][1] in dummyC):
            dummyL[i] = [1, 1, 0]
            dummyC.clear()
        elif (([1, 1, 0] == dummyL[i]).all() and passA[i][2] in dummyC):
            dummyL[i] = [1, 1, 1]
            dummyC.clear()
        if (([1, 1, 1] == dummyL[i]).all()) == True:
            for j in range(len(dummyL)):
                dummyL[j] = [0, 0, 0]
            return "pass" + str(i)

def handCheck(direct, finger, side: str):
    for i in range(4, 21, 4):
        x1, y1 = np.subtract(direct[i], direct[0])
        x2, y2 = np.subtract(direct[4], direct[17])
        x3, y3 = np.subtract(direct[9], direct[5])
        z1, z2, z3 = math.sqrt(pow(x1, 2) + pow(y1, 2)), math.sqrt(
            pow(x2, 2) + pow(y2, 2)), math.sqrt(pow(x3, 2) + pow(y3, 2))
        if (z3 != 0) & (i > 4):
            if (z1/z3) * 20 > 100:
                finger.append(f"{(i/4) - 1}{side}")
        if (z3 != 0) & (i == 4):
            if (z2/z3) * 20 > 100:
                finger.append(f"{(i/4) - 1}{side}")


def detectH():
    host = "broker.emqx.io"
    port = 1883
    
    cap = cv2.VideoCapture(0)

    arr = np.zeros((21, 2))
    passArray = [[3,2,1], [4,5,6]] # ? master, slave
    dummyCheck = []
    Cpass = ""
    dummyLen = np.zeros((len(passArray), len(passArray[0])))

    mpHands = mp.solutions.hands
    hands = mpHands.Hands()
    mpDraw = mp.solutions.drawing_utils
    
    data2JSON = [{"Qr": "", "Hands": "", "Face": "0"}]
    while cap.isOpened():
        finger = []
        check, frame = cap.read()
        imgRGB = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = hands.process(imgRGB)
        Qr = decoder(frame)
        if (Qr != None):
            data2JSON[0]["Qr"] = Qr
        if results.multi_hand_landmarks:
            for handLms in results.multi_hand_landmarks:
                for id, lm in enumerate(handLms.landmark):
                    h, w, c = frame.shape
                    cx, cy = int(lm.x * w), int(lm.y * h)
                    arr[id] = [cx, -cy]
                mpDraw.draw_landmarks(frame, handLms, mpHands.HAND_CONNECTIONS)
                if np.cross(np.subtract(arr[5], arr[0]), np.subtract(arr[17], arr[5])) > 0:
                    handCheck(arr, finger, "R")
                else:
                    handCheck(arr, finger, "L")
        Cpass = passCheck(finger, dummyCheck, dummyLen, passArray)
        if (Cpass != None):
            data2JSON[0]["Hands"] = Cpass
        
        if (data2JSON == [{"Qr": "Master", "Hands": "pass0", "Face": "face0"}] or data2JSON == [{"Qr": "Slave", "Hands": "pass1", "Face": "face1"}]):
            jsonNode = json.dumps(data2JSON[0])
            data2JSON = [{"Qr": "", "Hands": "", "Face": ""}]
            client = mqtt.Client()
            client.connect(host)
            client.publish("reai/pc", jsonNode)
            print(jsonNode)
        cv2.putText(frame, "Finger : ", (10, 70),
                    cv2.FONT_HERSHEY_PLAIN, 1, (218, 224, 159), 2)
        cv2.putText(frame, f"Finger Count: ", (10, 40),
                    cv2.FONT_HERSHEY_PLAIN, 2, (218, 224, 159), 3)
        if len(finger) != 0:
            cv2.putText(frame, f"{' '.join(finger)}", (85, 70),
                        cv2.FONT_HERSHEY_PLAIN, 1, (47, 209, 29), 2)
            cv2.putText(frame, f"{str(len(finger))}", (245, 40),
                        cv2.FONT_HERSHEY_PLAIN, 2, (47, 209, 29), 3)
        else:
            cv2.putText(frame, "None", (85, 70),
                        cv2.FONT_HERSHEY_PLAIN, 1, (57, 130, 247), 2)
            cv2.putText(frame, f"{str(len(finger))}", (245, 40),
                        cv2.FONT_HERSHEY_PLAIN, 2, (57, 130, 247), 3)
        if check == True:
            cv2.imshow("Video", frame)
            if cv2.waitKey(1) & 0xFF == ord('e'):
                break
        else:
            break
    cap.release()
    cv2.destroyAllWindows()
    
# def detectF():
#     cap = cv2.VideoCapture(0)
#     database_image = face_recognition.load_image_file("Phytes.jpg")
#     data_base_encoding = face_recognition.face_encodings(database_image)[0]

#     person_face_encodings = [data_base_encoding]
#     person_face_names = ["Phytes"]

#     data_locations = []
#     data_encodings = []
#     data_names = []
#     frameProcess = True

#     while True:
#         ret, frame = cap.read()
#         rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
#         data_locations = face_recognition.face_locations(rgb_frame)
#         for top, right, bottom, left in data_locations:
#             cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 4)
#             data_encodings = face_recognition.face_encodings(rgb_frame, data_locations)
#             data_names = []
#             for dc in data_encodings:
#                 matches = face_recognition.compare_faces(person_face_encodings, dc)
#                 name = "UNKNOWN"
#                 if True in matches:
#                     first_match_index = matches.index(True)
#                     name = person_face_names[first_match_index]

#                 data_names.append(name)
#                 cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (26, 174, 10), cv2.FILLED)
#                 font = cv2.FONT_HERSHEY_DUPLEX
#                 cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

#         frame = cv2.resize(frame, (600, 340))     
#         cv2.imshow('Video', frame)
        
#         if cv2.waitKey(25) == 13:
#             break

#     cap.release()
#     cv2.destroyAllWindows()
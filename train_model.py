import os
from ultralytics import YOLO
import numpy as np  # для работы с матрицами

current_dir = os.path.dirname(os.path.abspath(__file__))
data_path = os.path.join(current_dir, 'uav_v8.yaml')

if not os.path.exists(data_path):
    print(f"Путь не существует: {data_path}")
    exit()
else:
    print(f"Путь существует: {data_path}")
    
model = YOLO(os.path.join(current_dir, 'yolov8n.pt'))

imgsz = 640
epochs = 50
batch = 16

if __name__ == '__main__':
   results = model.train(
      data=data_path,
      imgsz=imgsz,   #1280
      epochs=epochs,
      batch=batch,        #8
      name='yolov8n_v8_UAV'
   )
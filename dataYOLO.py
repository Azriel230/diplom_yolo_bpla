
import os
import shutil
from sklearn.model_selection import train_test_split

pwd = os.path.dirname(os.path.realpath(__file__)) 
input_folder = os.path.join(pwd, "../UOV/5g/train")

if not os.path.exists(input_folder):
    print(f"Путь не существует: {input_folder}")
    exit()
else:
    print(f"Путь существует: {input_folder}")

images = [os.path.join(input_folder, x) for x in os.listdir(input_folder)  if  x[-3:]=="png"]
annotations = [os.path.join(input_folder, x) for x in os.listdir(input_folder) if  x[-3:]=="txt"]

images.sort()
annotations.sort()

# Проверка на соответствие количества изображений и меток
if len(images) != len(annotations):
    print("Количество изображений и меток не совпадает.")
    exit()

# print(len(images))
# print(len(annotations))

train_images, val_images, train_annotations, val_annotations = train_test_split(images, annotations, test_size = 0.3, random_state = 1)
val_images, test_images, val_annotations, test_annotations = train_test_split(val_images, val_annotations, test_size = 0.8, random_state = 1)

# #Utility function to move images 
def move_files_to_folder(list_of_files, destination_folder):
    # Создание папки, если она не существует
    os.makedirs(destination_folder, exist_ok=True)
    
    for f in list_of_files:
        try:
            shutil.copy(f, destination_folder)
        except Exception as e:
            print(f"Ошибка при копировании файла {f}: {e}")
            assert False

# Move the splits into their folders
output_dir = pwd + "/dataset_v1.1/"
move_files_to_folder(train_images, output_dir + 'images/train')
move_files_to_folder(val_images, output_dir + 'images/val/')
move_files_to_folder(test_images, output_dir + 'images/test/')
move_files_to_folder(train_annotations, output_dir + 'labels/train/')
move_files_to_folder(val_annotations, output_dir + 'labels/val/')
move_files_to_folder(test_annotations, output_dir + 'labels/test/')
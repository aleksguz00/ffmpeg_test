# Тестовое по FFmpeg

## 1. Клонировать проект, перейти в директорию проекта
## 2. Создать директорию для сборки и перейти в неё
    mkdir build && cd build
## 3. Собрать проект
    cmake ..
    make 
## 4. Запустить программу, передав ей видеофайл
    ./video /path/to/video_file.mp4
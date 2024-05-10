### Компиляция 
g++ main.cpp -o main -std=c++11 -stdlib=libc++   

### Компиляция с БД
g++ main.cpp -o main -lsqlite3 -std=c++11 -stdlib=libc++ 

### Запуск 
bash ./main
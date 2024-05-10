import sqlite3
import random

# Создание новой базы данных SQLite
conn = sqlite3.connect('car.db')
c = conn.cursor()

# Создание таблицы Cars
c.execute('''
CREATE TABLE IF NOT EXISTS Cars (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type INTEGER NOT NULL,
    brand INTEGER NOT NULL,
    price INTEGER NOT NULL
)
''')

# Функция для генерации случайных данных автомобилей
def generate_cars(num_cars):
    cars = []
    for _ in range(num_cars):
        type = random.randint(0, 3)  # Тип кузова (0=Sedan, 1=Pickup, 2=Coupe, 3=Cabriolet)
        brand = random.randint(0, 3)  # Марка автомобиля (0=Volvo, 1=Mersedes, 2=BMW, 3=Skoda)
        price = random.randint(0, 4)  # Ценовая категория (0=Very_Low, 1=Low, 2=Medium, 3=High, 4=Very_High)
        cars.append((type, brand, price))
    return cars

# Генерация данных для 50 автомобилей
car_data = generate_cars(50)

# Вставка данных в таблицу
c.executemany('INSERT INTO Cars (type, brand, price) VALUES (?, ?, ?)', car_data)

# Сохранение изменений и закрытие соединения с базой данных
conn.commit()
conn.close()

print("База данных успешно создана и заполнена.")

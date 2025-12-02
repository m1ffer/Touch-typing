import json
import sys
import os


def process_file(input_file, output_dir):
    with open(input_file, 'r', encoding='utf-8') as f:
        data = json.load(f)

    # Определяем тип данных (цитаты или слова)
    if 'quotes' in data:
        items = data['quotes']
        item_type = 'quotes'
        length_func = lambda x: x['length']  # для цитат берем поле length
        text_func = lambda x: x['text']
    elif 'words' in data:
        items = data['words']
        item_type = 'words'
        length_func = lambda x: len(x)  # для слов считаем длину строки
        text_func = lambda x: x
    else:
        raise ValueError("Файл должен содержать 'quotes' или 'words'")

    # Вычисляем среднюю длину
    total_length = sum(length_func(item) for item in items)
    average_length = total_length / len(items)

    print(f"Обработано {item_type}: {len(items)}")
    print(f"Средняя длина: {average_length:.2f}")

    # Разделяем элементы
    short_items = [item for item in items if length_func(item) < average_length]
    long_items = [item for item in items if length_func(item) > average_length]

    # Создаем директорию
    os.makedirs(output_dir, exist_ok=True)

    # Определяем имена файлов в зависимости от типа
    if item_type == 'quotes':
        short_filename = 'shortQuotes.json'
        long_filename = 'longQuotes.json'
    else:
        short_filename = 'shortWords.json'
        long_filename = 'longWords.json'

    # Сохраняем файлы
    short_path = os.path.join(output_dir, short_filename)
    long_path = os.path.join(output_dir, long_filename)

    # Сохраняем с сохранением структуры
    short_data = data.copy()
    short_data[item_type] = short_items
    with open(short_path, 'w', encoding='utf-8') as f:
        json.dump(short_data, f, ensure_ascii=False, indent=2)

    long_data = data.copy()
    long_data[item_type] = long_items
    with open(long_path, 'w', encoding='utf-8') as f:
        json.dump(long_data, f, ensure_ascii=False, indent=2)

    print(f"Короткие элементы сохранены в: {short_path}")
    print(f"Длинные элементы сохранены в: {long_path}")
    print(f"Коротких: {len(short_items)}")
    print(f"Длинных: {len(long_items)}")

    # Примеры
    if short_items:
        sample_short = [text_func(item) for item in short_items[:3]]
        print(f"Пример коротких: {sample_short}")
    if long_items:
        sample_long = [text_func(item) for item in long_items[:3]]
        print(f"Пример длинных: {sample_long}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Использование: python processor.py <input_file.json> <output_directory>")
        print("Пример: python processor.py words.json ./results")
        sys.exit(1)

    input_file = sys.argv[1]
    output_dir = sys.argv[2]

    if not os.path.isfile(input_file):
        print(f"Ошибка: Файл '{input_file}' не найден")
        sys.exit(1)

    process_file(input_file, output_dir)
    print("Обработка завершена успешно!")
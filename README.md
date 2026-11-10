# CMake CLI Calculator

Консольный калькулятор, написанный по ТЗ (без STL, с парсингом `getopt`). Автоматически подтягивает математическую библиотеку и настройки линтеров через `FetchContent`.

## Зависимости

```bash
sudo apt update && sudo apt install cmake g++ clang-format clang-tidy
```

## Сборка и форматирование

```bash
# Сборка проекта
rm -rf build && cmake -B build && cmake --build build

# Автоматическое форматирование кода (clang-format)
cmake --build build --target format
```

## Установка и запуск

```bash
# Глобальная установка в систему
sudo cmake --build build --target install

# Примеры использования из любой папки
calculator -h
calculator -a 5 -b 5 -o +
calculator -a 5 -o !
calculator -a 13 -o !
```


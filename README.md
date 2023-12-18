# Websocket Server-Client

## Сборка проекта

Для сборки проекта необходимо установить Microsoft WDK (Windows Driver Kit). Вы можете скачать его по следующей ссылке: [Download WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk#download-icon-enterprise-wdk-ewdk)

1. Запустите `LaunchBuildEnv.cmd`.
2. Перейдите в каталог проекта с помощью команды в консоли: `cd /d your_path`.
3. Запустите команду для сборки проекта: `msbuild WebSocketServer.sln /p:Configuration=Release /p:Platform="Win32"`.

## Запуск WebSocket сервера

1. После успешной сборки перейдите в каталог `Release` в корневой папке проекта.
2. Запустите `WebSocketServer.exe`.

Должно появиться сообщение:
Press ESCAPE to terminate program,
Starting up TCP server


## Запуск WebSocket клиента

1. Откройте файл `client.py` в корневой папке проекта с помощью любого текстового редактора.
2. Замените переменные `HOST` и `initial_message` на свой IP-адрес. IP-адрес можно узнать с помощью команды `ipconfig` в командной строке.
3. Запустите `client.py` и введите любое сообщение в терминале. В окне, где запущен `WebSocketServer.exe`, должно появиться сообщение, которое вы написали.

Если сообщение отображается неправильно, выполните команду `chcp 65001`.

---
**Примечание:** Если у вас возникают проблемы, обратитесь к документации WDK и удостоверьтесь, что все необходимые зависимости установлены.



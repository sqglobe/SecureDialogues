# SecureDialogues

Приложение добавляет уровень шифрования
сообщений, которые передаются через такие сервисы, как **Gmail**.

Каждое сообщение снабжается электронной подписью. Это позволяет 
выполнить аутентификацию собеседника и проверить целостность сообщения.

Вся корреспонденция, которая пересылается между пользователями,
шифруется 256-битным сеансовым ключом по алгоритму **AES**. Они не сохраняются
на долговременном носителе и генерируются каждый раз, как только пользователи 
создают диалог. Это значительно повышает стойкость системы к методам криптоанализа.

Приложение выполняет авторизацию на сторонних сервисах для передачи сообщений по протоколу 
**OAuth2**. Такой подход позволяет пользователям управлять доступом приложения к их аккаунту, 
а так же исключить риск компрометации паролей. 

Все данные, которые приложение сохраняет на жестком диске, шифруются с помощью 
256-битного ключа по протоколу **AES**. Данный ключ генерируется из пароля, который задает пользователь.

На данный момент доступна передача сообщений через сервис **Gmail**, но ведутся работы по расширению этого списка.

Дополнительная информация находится в [wik проекта](https://github.com/sqglobe/SecureDialogues/wiki/Главная).
Документация проекта - в парке **doc**

## Зависимости

Для выполнения сборки необходимы:

* cmake версии **3.8** и выше.
* Qt5
* git
* Boost (system) версии **1.58.0** и выше
* [asio Boost](https://think-async.com/Asio/)
* [libcurl] (https://github.com/curl/curl) версии **7.50.0** и выше
* [vmime] (https://www.vmime.org/) начиная с коммита **e2fc191** и выше.
* [persistent-storage](https://github.com/sqglobe/persistent-storage) версии **1.0.0** и выше.

Ниже перечислены модули, которые так же используются при сборке, но они либо добавлены в репозиторий,
либо будут склонированы во время сборки:

* [CryptoPP](https://www.cryptopp.com/)
* [crossguid](https://github.com/graeme-hill/crossguid)
* [spdlog](https://github.com/gabime/spdlog)
* [nlohmann json] (https://github.com/nlohmann/json)
* [eventpp] (https://github.com/wqking/eventpp)
* [fmt] (https://github.com/fmtlib/fmt)

Так же для успешного разбора **CMakeLists.txt**  может потребоваться задать переменную **CMake** - `CMAKE_PREFIX_PATH`.


Для генерации документации понадобятся:

* [asciidoctor-pdf](https://asciidoctor.org/docs/asciidoctor-pdf)
* [plantuml](http://plantuml.com/)

Более подробно про процесс сборки на [wiki](https://github.com/sqglobe/SecureDialogues/wiki/%D0%A1%D0%B1%D0%BE%D1%80%D0%BA%D0%B0-%D0%BF%D1%80%D0%B8%D0%BB%D0%BE%D0%B6%D0%B5%D0%BD%D0%B8%D1%8).

## Форматирование кода

Для форматирования кода используется **clang-format**. 
В корне проекта находится файл **.clang-format** с описанием правил форматирования. Для того, чтобы применить их для всего проекта 
достаточно выполнить команду:
```
find . \( -name '*.cpp' -o -name '*.h' \) -exec clang-format -i {} \;
```

Добавлено автоматическое форматирование кода с использованием **git-hook**. Для того, чтобы это заработало, необходимо выполнить команду:
```
git config core.hooksPath .githooks
```

## Docker

Для сборки проекта может использоваться Docker.
Образы находятся в **docker**.

**docker image** создается командой

```
docker build -t <image-name> --file docker/<docker file> .
```

Где:

* **image-name** имя образа
* **docker file** docker-файл, который используется для сборки под конкретную платформу.

Проект собирается командой:

```
docker run  --mount type=bind,source=<result folder>,target=/app/res --mount type=bind,source=<sources>,target=/app/src <image-name>
```

Где:

* **result folder** папка, в которую будут скопированы бинарные файлы и документация. Директория должна существовать.
* **sources** путь к исходникам
* **image-name** ранее созданный docker-образ

Пример:

```
docker run  --mount type=bind,source=/tmp/github/res,target=/app/res --mount type=bind,source=/tmp/github/SecureDialogues,target=/app/src secure-dialogues-ubuntu-bionic
```

Более подробно на [wiki](https://github.com/sqglobe/SecureDialogues/wiki/docker)

## Запуск приложения в Docker

Добавлена возможность запустить приложение в контейнере.
Для этого в **Makefile** в дирректории **docker** включена цель **docker**. Эта цель собирает проект под **Ubuntu 18.04**, используя **ubuntu-bionic.docker**.
Затем создается **image** и запускается контейнер с приложением внутри. 

Возможна такая ситуация, когда расширение [Xsecurity](https://www.x.org/releases/X11R7.7/doc/man/man7/Xsecurity.7.xhtml) блокирует неавторизованные подключения к X-серверу.
Решить проблему поможет команда:

```
xhost +local:
```

## Локализация

В приложении доступна локализация для русского языка. Для Ubuntu может потребоваться выполнить команду:

```
sudo locale-gen ru_RU
```

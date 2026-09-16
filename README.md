# LatencyX

Herramienta ligera en C++ para la optimizacion de latencia del sistema, ajuste del temporizador de Windows y gestion de procesos en ejecucion de juegos.

## Descripcion

LatencyX es una utilidad orientada al rendimiento que interactua directamente con las API de Windows (Win32) para reducir el latido del temporizador del sistema, elevar la prioridad de ejecucion de los procesos de juegos y ajustar la afinidad de nucleos de la CPU para minimizar el micro-stuttering y la latencia de entrada (input lag).

## Funcionalidades principales

- Ajuste de alta precision del System Timer (resolucion de temporizador de 1.0 ms / 0.5 ms via timeBeginPeriod).
- Elevacion automatica de prioridad de procesos detectados a HIGH_PRIORITY_CLASS.
- Asignacion de mascara de afinidad de CPU para garantizar ejecucion en nucleos fisicos.
- Consumo de recursos minimo (ejecutable autocontenido, sin dependencias externas pesadas).

## Requisitos de compilacion

- Sistema operativo Windows 10 u 11 (64 bits).
- Compilador compatible con C++17 (MSVC, MinGW-w64 o Clang).
- CMake 3.16 o superior.

## Compilacion e instalacion

1. Clonar el repositorio:
   git clone https://github.com/tu-usuario/LatencyX.git
   cd LatencyX

2. Generar los archivos de construccion con CMake:
   cmake -B build -DCMAKE_BUILD_TYPE=Release

3. Compilar el proyecto:
   cmake --build build --config Release

El binario ejecutable `LatencyX.exe` se encontrara en el directorio `build/Release` o `build/`.

## Uso

Ejecutar `LatencyX.exe` con privilegios de administrador para permitir la modificacion de prioridades de procesos del sistema y la alteracion de la resolucion del temporizador.


## License & Acknowledgments

This project uses **Dear ImGui**, licensed under the terms of the MIT License:

```text
The MIT License (MIT)

Copyright (c) 2014-2026 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Check License file for more info.
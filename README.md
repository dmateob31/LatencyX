# LatencyX

Herramienta ligera en C++ para la optimización de la latencia del sistema, ajuste del temporizador de Windows y gestión avanzada de procesos en juegos.

---

## Descripción

LatencyX es una utilidad de alto rendimiento orientada a la optimización del sistema que interactúa directamente con las API nativas de Windows (Win32). Su objetivo principal es reducir la latencia de entrada (input lag), minimizar el micro-stuttering y maximizar el rendimiento del hardware mediante la manipulación precisa de temporizadores, prioridades de ejecución y la afinidad de la CPU.

---

## Características Principales

| Funcionalidad | Descripción |
| :--- | :--- |
| Control del System Timer | Ajuste de alta precisión del temporizador del sistema (resoluciones de 1.0 ms / 0.5 ms mediante `timeBeginPeriod`). |
| Gestión de Procesos | Elevación automática de la prioridad de ejecución a `HIGH_PRIORITY_CLASS` para los procesos seleccionados. |
| Afinidad de CPU | Asignación de máscaras de afinidad para priorizar núcleos físicos y optimizar el rendimiento de procesamiento. |
| Purga de Memoria RAM | Liberación de memoria física en segundo plano para mantener la estabilidad operativa. |
| Interfaz Gráfica | Interfaz de usuario construida con Dear ImGui y aceleración por DirectX 11. |
| Eficiencia de Recursos | Ejecutable autocontenido, diseñado para mantener un consumo mínimo de recursos. |

---

## Requisitos del Sistema

- **Sistema Operativo:** Windows 10 u 11 (64 bits).
- **Compilador:** Compatible con C++17 (MSVC, MinGW-w64 o Clang).
- **Herramientas de construcción:** CMake 3.16 o superior.

---

## Compilación e Instalación

1. Clonar el repositorio:
   ```bash
   git clone [https://github.com/dmateob31/LatencyX.git](https://github.com/dmateob31/LatencyX.git)
   cd LatencyX

2. Generar los archivos de construccion con CMake:
   cmake -B build -DCMAKE_BUILD_TYPE=Release

3. Compilar el proyecto:
   cmake --build build --config Release

El binario ejecutable `LatencyX.exe` se encontrara en el directorio `build/Release` o `build/`.

Uso
Dirígete a la carpeta donde se compiló el binario.

Ejecute LatencyX.exe con privilegios de administrador.

Nota: Los privilegios de administrador son obligatorios para permitir la modificación de las prioridades de los procesos del sistema y la alteración de la resolución del temporizador.

Contribuciones
Las contribuciones al proyecto son bienvenidas. Si desea proponer mejoras, reportar incidencias o incorporar nuevas características:

Realice un fork del repositorio.

Cree una rama para su funcionalidad (git checkout -b feature/NombreFuncionalidad).

Confirme sus cambios (git commit -m 'Descripción de los cambios').

Suba la rama (git push origin feature/NombreFuncionalidad).

Abra una solicitud de extracción (Pull Request).

Licencia
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

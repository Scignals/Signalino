# Prueba de Signalino Suite 0.10.0 Preview

Guía de instalación y prueba en equipos Windows y Linux. Preparada el 17 de
septiembre de 2026. Primero hacer pruebas de banco, sin electrodos en personas.
No hace falta recompilar la Suite ni actualizar el firmware para esta prueba.

## 1. Descargar

- [Windows x64: ZIP completo](https://github.com/Scignals/Signalino/releases/download/v0.10.0/Signalino-Suite-0.10.0-Windows-x64-preview.zip).
- [Ubuntu/Debian amd64: paquete DEB](https://github.com/Scignals/Signalino/releases/download/v0.10.0/signalino-suite_0.10.0_amd64.deb).
- [Mac Apple Silicon: DMG](https://github.com/Scignals/Signalino/releases/download/v0.10.0/Signalino-Suite-0.10.0-macOS-arm64-preview.dmg).
- [Android 12+: Bridge y demo HepatoSense](https://github.com/Scignals/Signalino/releases/download/v0.10.0/Signalino-Android-0.4.0-preview.zip).
- [Sumas SHA-256 de escritorio](https://github.com/Scignals/Signalino/releases/download/v0.10.0/SHA256SUMS.txt).
- [Notas completas y limitaciones](https://github.com/Scignals/Signalino/blob/main/docs/releases/0.10.0.md).
- [Informe de validación automática](https://github.com/Scignals/Signalino/blob/main/docs/releases/VALIDACION_0.10.0_2026-09-17.md).

La validación automática de Linux utiliza Ubuntu 22.04 x64. No equivale a probar
todas las distribuciones. Windows se prueba en un entorno x64 de GitHub; siguen
pendientes el adaptador Bluetooth, los permisos y la interfaz del PC concreto.

## 2. Instalar

### Windows

1. Salir de la Suite anterior con Quit, también desde el icono junto al reloj.
2. Extraer TODO el ZIP a una carpeta nueva. No ejecutar desde dentro del ZIP ni
   copiar solo `SignalinoSuite.exe`: necesita los archivos que lo acompañan.
3. Abrir `SignalinoSuite.exe`. Incluye el runtime .NET; no requiere Visual Studio
   ni instalar herramientas de compilación.
4. La preview no tiene firma comercial. Revisar procedencia y hash si Windows
   muestra un aviso. No desactivar las protecciones del sistema globalmente.

La adquisición física por USB/Classic/BLE no requiere Python. El mock de
diagnóstico sí necesita Python 3: se puede seleccionar su ejecutable en Settings.

### Linux

Desde la carpeta donde se descargó el paquete:

```sh
sudo apt install ./signalino-suite_0.10.0_amd64.deb
```

Abrir Signalino desde el menú de aplicaciones, o ejecutar:

```sh
signalino_linux_suite
```

El paquete declara sus dependencias. La interfaz utiliza X11 o XWayland; el
indicador cerca del reloj depende del soporte AppIndicator del escritorio.
Si USB falla por permisos o Bluetooth no está disponible, conservar el mensaje
exacto y la distribución utilizada; no resolverlo ejecutando toda la Suite como root.

## 3. Prueba sin amplificador

1. Arrancar sin dispositivo conectado. El visor abierto no debe por sí solo
   indicar recepción de datos ni mantener un dispositivo antiguo seleccionado.
2. En Diagnostics, iniciar el mock. Abrir el visor y comprobar señales móviles,
   ocho canales y frecuencia coherente. El mock sintético requiere solo Python 3;
   para reproducir EDF hacen falta además numpy y pyedflib en ese Python.
3. Abrir y cerrar impedancias: deben aparecer valores simulados y después volver
   el EEG. Esto NO calibra ni valida impedancias del amplificador real.
4. En Settings, elegir carpeta, nombre de grabación y perfil. Activar la opción
   EDF+. El perfil de ejemplo solo vale si coincide con la conexión de canales.
5. Grabar unos 30 segundos, parar y esperar a que termine la exportación. Revisar
   que haya datos y EDF, con nombres de canales y frecuencia correctos.
6. Repetir con el mismo nombre base: no debe sobrescribir la toma anterior.
7. Detener el mock. La señal y el estado de recepción deben caducar; que otro
   programa aún enumere una LSL antigua no demuestra que esté recibiendo muestras.
8. Salir con Quit, volver a abrir y comprobar que los ajustes se conservan.

Si esto falla, guardar el log antes de pasar a Bluetooth: el problema puede ser
de instalación, Python, permisos de archivos o interfaz, no del amplificador.

## 4. Prueba con amplificador

Probar UN transporte cada vez. Antes, detener el mock y desconectar el amplificador
de la Suite del Mac, Android u otros programas que puedan estar usándolo.

| Equipo | Transporte que probar |
| --- | --- |
| Signalino original Teensy 3.6 | USB; Bluetooth Classic con el módulo emparejado |
| Signalino S3 | USB; BLE |

No confundir el HC-06/Classic con el BLE del S3. Para Classic, emparejar primero
en el sistema operativo. Comprobar el nombre y DEVICE_ID del equipo elegido.

Para cada transporte:

1. Conectar y comprobar que realmente llegan datos, no solo un estado de conexión.
2. Abrir el visor correcto y confirmar canalización, frecuencia y fuente de señal
   (ADS, test o síntesis). Señal plana no significa necesariamente fallo de LSL.
3. Mantener recepción cinco minutos. Anotar interrupciones y pérdida indicada;
   distinguir variación de refresco gráfico de ausencia real de muestras.
4. Abrir/cerrar impedancias y comprobar retorno del EEG. No usar los valores
   absolutos como prueba de calibración en esta preview.
5. Grabar dos minutos, parar y revisar el EDF con un lector independiente.
6. Sin una grabación activa, desconectar/apagar el dispositivo. Comprobar que
   desaparece el estado de recepción. Reconectar y anotar si es automático o
   requiere una acción del usuario: la recuperación aún no es igual en todos los SO.
7. Salir con Quit. Comprobar que la Suite no sigue reteniendo la conexión.

En Windows hay un antecedente de BLE que descubría el equipo pero no recibía
notificaciones con un adaptador concreto. No está demostrado que esta release
lo resuelva: esta prueba debe confirmarlo, sin cambiar a USB silenciosamente.

## 5. Qué anotar

Copiar este bloque para cada equipo/transporte:

```text
Fecha:
Suite: 0.10.0 Preview
Sistema y versión:
Modelo del PC y adaptador Bluetooth:
Amplificador / DEVICE_ID / firmware:
Transporte: mock / USB / Classic / BLE
Fuente: ADS / test / síntesis / EDF replay
Conecta y recibe datos:
Visor, canales y frecuencia:
Impedancias y vuelta al EEG:
Grabación y EDF revisado:
Pérdidas o pausas observadas:
Desconexión y recuperación:
Quit libera la conexión:
Error literal y log, si falla:
```

No publicar registros de pacientes, identificadores clínicos ni configuraciones
con información privada en incidencias de GitHub.

## Android y firmware

Android tiene versión independiente, 0.4.0 Preview. Leer el `ANDROID_README.md`
del ZIP antes de instalar: su firma difiere de los APK de desarrollo. No
desinstalar una versión antigua sin exportar y verificar antes sus grabaciones.

Los firmwares compilados no forman parte de esta prueba ni de las descargas
públicas actuales. Consultar [el estado de firmware](https://github.com/Scignals/Signalino/blob/main/docs/firmware/README.md).

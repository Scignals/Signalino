# Validación de las descargas públicas 0.10.0

Fecha: 17 de septiembre de 2026. Resultado: comprobaciones automáticas superadas.
Se han probado los paquetes ya publicados, no binarios recompilados para esta
comprobación. Continúan siendo una preview, no una validación clínica.

## Resultados

| Plataforma | Artefacto probado | Comprobaciones superadas |
| --- | --- | --- |
| Windows x64, runner Windows 2022 | ZIP público extraído completo | SHA-256, arranque de la Suite en modo de prueba, mock -> driver -> LSL, impedancias y retorno a EEG, grabación con datos, cierre y limpieza; lector EEG/sensores/markers; exportación EDF+ comprobada con MNE |
| Ubuntu 22.04 amd64, runner Linux | DEB público instalado con apt | SHA-256, dependencias/instalación, Suite en modo de prueba, mock, impedancias, desconexión, permisos y limpieza IPC; lector EEG/sensores/markers; grabación/exportación EDF+ comprobada con MNE |
| macOS Apple Silicon, Mac local | DMG público descargado de nuevo y montado solo lectura | SHA-256, integridad DMG, verificación de firma ad-hoc; ejecutables del propio paquete: lector EEG/sensores/markers y grabación/exportación EDF+ comprobada con MNE |

Las pruebas de grabación/exportación incluyen datos sintéticos, huecos/anotaciones
y frecuencias de 250, 500 y 1000 Hz. No se ha probado una adquisición física a
esas frecuencias en esta ejecución.

Windows/Linux: ejecución `35200243847` de `Scignals/Toydriver`, revisión de pruebas
`dfc54e73755db47664bdc0f30defb2a2ad6e42f1`; ambos trabajos terminaron correctamente.
Los tests están en el repositorio privado; este documento publica el alcance y
el resultado, no grabaciones ni material interno. Las revisiones que generaron
los instaladores siguen identificadas en el `BUILD_INFO.json` original.

La firma ad-hoc del Mac es íntegra, pero no equivale a notarización comercial.
En esta comprobación local no se sustituyó ni reinició la Suite instalada.

## Identidad de los paquetes

```text
2bb45834d6e0360d1ec71406a9d236a883c3bbd36c5f69864ecf3c93a9e7fef6  Signalino-Suite-0.10.0-Windows-x64-preview.zip
e3e0d417c400229096e2faf205100f47bf01d44821852a74d8626919fe3f29bf  signalino-suite_0.10.0_amd64.deb
2ca0123fb7e5322901ff0a010b1c02f0a375f27cd2ac341b34059412316c22f8  Signalino-Suite-0.10.0-macOS-arm64-preview.dmg
```

Los instaladores, `BUILD_INFO.json` y `SHA256SUMS.txt` originales no se han
modificado. Este informe y la guía de pruebas son documentación adicional.

## Android

Bridge y demo HepatoSense 0.4.0 Preview ya están publicados. Se mantiene la
evidencia de la ejecución `35194914264`: compilación, 36 pruebas JVM, firma
concordante e instalación/comunicación/análisis/EDF en emulador Android API 35.
No se ha repetido ese emulador en esta comprobación de escritorio. Persisten
30 avisos de lint, sin errores, y la prueba física de los APK de distribución
sigue pendiente. Consultar las notas sobre migración de firma antes de instalarlos.

## Pendiente de probar en los PCs

- Radios USB, Classic y BLE reales; permisos, emparejamiento y adaptadores.
- Caso de Windows BLE que conecta pero no recibe notificaciones.
- Recuperación al desconectar/reconectar y grabación sostenida.
- Interfaz visible, escalado de pantalla e indicador cerca del reloj en cada escritorio.
- Correspondencia de electrodos/perfil y calibración real de impedancias.

Guía y plantilla para anotar resultados:
[pruebas Windows/Linux](https://github.com/Scignals/Signalino/blob/main/docs/PRUEBAS_0.10.0.md).

## Qué no se ha publicado

No se publican emuladores de ESP32/ADS1299, fuentes privadas, borradores de
licencias ni datos de investigación. Los firmwares del amplificador aún necesitan
cerrar su paquete de recuperación, obligaciones de dependencias y validación de
la imagen exacta en el hardware correspondiente. Compilar no cierra esos puntos.

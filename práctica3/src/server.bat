:: cambiar c: y c:\hlocal\TPV2 por la ruta de tu proyecto

c:
cd c:\hlocal\TPV2

:: añdir la ruta de las librerias dll de SDL a la variable de entrono PATH

set PATH=SDL\bin;%PATH%
 
:: ejecutar en modo servidor

bin\x64\Debug\TPV2.exe server 2000


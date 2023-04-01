probar() {
  echo "Test de ./$1 $2"
  ( ./$1 $2 > res.txt ) >& err.txt
  RC=$?
  echo "La salida estandar es:"
  cat res.txt
  if [ $RC -ne 0 ]
  then
    echo
    echo "*** El codigo de retorno fue $RC. Debio ser 0 ***"
    echo "El codigo de retorno es el valor que retorna la funcion main"
    echo "o el argumento de la funcion exit"
    exit 1
  fi
  diff res.txt $3
  if [ $? -ne 0 ]
  then
    echo
    echo "*** La salida estandar es incorrecta ***"
    echo "Debio ser:"
    cat $3
    RES=`cat res.txt`
    REFER=`cat $3`
    echo "Si la salida parece ser igual, instale xxdiff con:"
    echo "    sudo apt-get install xxdiff"
    echo "y luego compare los caracteres invisibles con:"
    echo "xxdiff res.txt $3"
    exit 1
  fi
  diff err.txt empty.txt
  if [ $? -ne 0 ]
  then
    echo
    echo "*** La salida estandar de errores es incorrecta ***"
    echo "Debio estar vacia"
    exit 1
  fi
  echo Aprobado
}

probarError() {
  echo "Test de ./$1 $2"
  echo "Deberia señalar que la cola esta vacia."
  ( ./$1 $2 > res.txt ) >& err.txt
  RC=$?
  echo "La salida estandar de errores es:"
  cat err.txt
  if [ $RC -eq 0 ]
  then
    echo
    echo "*** El codigo de retorno fue 0. Debio ser distinto de 0 ***"
    echo "El codigo de retorno es el valor que retorna la funcion main"
    echo "o el argumento de la funcion exit"
    exit 1
  fi
  diff err.txt $3
  if [ $? -ne 0 ]
  then
    echo
    echo "*** La salida estandar de errores es incorrecta ***"
    echo "Debio ser:"
    cat $3
    RES=`cat err.txt`
    REFER=`cat $3`
    echo "Si la salida parece ser igual, instale xxdiff con:"
    echo "    sudo apt-get install xxdiff"
    echo "y luego compare los caracteres invisibles con:"
    echo "xxdiff err.txt $3"
    exit 1
  fi
  diff res.txt empty.txt
  if [ $? -ne 0 ]
  then
    echo
    echo "*** La salida estandar es incorrecta ***"
    echo "Debio estar vacia"
    exit 1
  fi
  echo Aprobado
}
                

echo cp cola-orig.txt cola.txt
cp cola-orig.txt cola.txt
probar $1 cola.txt juan.txt
echo ----------------------------------
probar $1 cola.txt ximena.txt
echo ----------------------------------
probar $1 cola.txt diego.txt
echo ----------------------------------
probar $1 cola.txt pedro.txt
echo ----------------------------------
echo "Se compara cola.txt con cola-final.txt"
diff cola.txt cola-final.txt
if [ $? -ne 0 ]
then
  echo
  echo "*** El contenido final de cola.txt es incorrecto ***"
  echo "El contenido es:"
  echo
  cat cola.txt
  echo
  echo "Debio ser:"
  echo
  cat cola-final.txt
  echo
  echo "Si los archivos parecen ser iguales, instale xxdiff con:"
  echo "sudo apt-get install xxdiff"
  echo "y luego compare los caracteres invisibles con:"
  echo "xxdiff cola.txt cola-final.txt"
  exit 1
fi
echo Aprobado
echo ----------------------------------
echo Probando extraer cuando la cola esta vacia
probarError $1 cola.txt error.txt
echo ----------------------------------
echo Probando cuando la cola esta en otro archivo
echo cp cola-orig.txt otra-cola.txt
cp cola-orig.txt otra-cola.txt
probar $1 otra-cola.txt juan.txt
echo ----------------------------------
echo Probando que se diagnostique un error cuando el archivo no existe
echo "Test de ./$1 no-existe.txt"
( ./$1 no-existe.txt > res.txt ) >& err.txt
RC=$?
echo "La salida estandar de errores es:"
cat err.txt
if [ $RC -eq 0 ]
then
  echo
  echo "*** El codigo de retorno fue 0. Debio ser distinto de 0 ***"
  echo "El codigo de retorno es el valor que retorna la funcion main"
  echo "o el argumento de la funcion exit"
  exit 1
fi
ERR=`cat err.txt`
LEN=`cat err.txt | wc -l`
if [ "$LEN" != "1" ]
then
  echo
  echo "*** La salida estandar de errores es incorrecta ***"
  echo "Se debio diagnosticar el error."
  exit 1
fi
cmp res.txt empty.txt
if [ $? -ne 0 ]
then
  echo
  echo "*** La salida estandar es incorrecta ***"
  echo "Debio estar vacia."
  exit 1
fi
echo Aprobado

echo
echo "Felicitaciones: Todos los tests aprobados"

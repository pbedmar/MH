Title: Practica final de Metaheurísticas
Author: Pedro Bedmar López
Year: 2022


En este fichero zip encontramos tres elementos en su raíz:

	- codigo/ -> contiene el código implementado así como los archivos de cec'17
		* big-bang_big-crunch_original.cpp -> implementación original tal y como aparece en la publicación (implementada por mi)
		* big-bang_big-crunch_ls.cpp -> versión memética utilizando solis wets
		* big-bang_big-crunch_bmb.cpp -> mejora con búsqueda multiarranque básica

	- diapositivas.pdf -> diapositivas explicativas de la metaheurística

	- memoria.pdf -> memoria con la discusión de los resultados



Para ejecutar el código:

	- Abrimos una terminal al mismo nivel que este archivo.
	- Ejecutamos cmake --build codigo --target all 
	- Ejecutamos cd codigo/
	- Podemos lanzar cualquiera de las tres versiones con ./original, ./ls y ./bmb


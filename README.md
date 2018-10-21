# GaussMethod
Projeto de Tópicos em Orientação a Objetos para calcular sistemas lineares com o método de Gauss, desenvolvido em C


- Para solucionar um sistema:
   - *1) Crie um arquivo .txt com o sistema, suas variáveis e coeficientes ;
   - *2) Abra o programa e digite o endereço do arquivo + ".txt";
   - *OBS: Se o coeficiente da incógnita for 1, você deverá digitá-lo no arquivo, vide exemplos;
   
   
- Como funciona o método:
    - O método de Gauss é uma generalização do método de redução, que consiste em eliminar uma incógnita nos sistemas de duas equações com duas incógnitas;
    - Consiste na aplicação sucessiva do método de redução, utilizando os critérios de equivalência de sistemas, para transformar a matriz ampliada com os termos independentes ( A* ) em uma matriz triangular, de modo que cada linha (equação) tenha uma incógnita a menos que a imediatamente anterior;
    - Obtemos assim um sistema, que chamaremos escalonado, tal que a última equação tem  uma única incógnita, a penúltima duas incógnitas, a antepenúltima três incógnitas, ...,  e a primeira todas as incógnitas;
    - Fonte: http://www.igm.mat.br/aplicativos/index.php?option=com_content&view=article&id=310%3Amgauss&catid=41%3Aconteudosal&Itemid=38;

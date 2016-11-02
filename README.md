# Horn Sat (logic)

Este é um pequeno projeto construído para a disciplina "Lógica para programação", do curso de Engenharia da Computação no CIn - UFPE.

A proposta deste projeto é receber como entrada uma sequência de clausulas de horn e resolvelas através do método da resolução.

O Projeto foi feito de acordo com a seguinte especificação:
```sh

O Projeto é um programa que aplica o Método da Resolução em Expressões bem-formadas. Para tal, haverá a necessidade que seu programa verifique se a expressão de entrada está na FNC e se todas as cláusulas são de Horn. É garantido que as expressões serão bem-formadas.

O programa poderá ser feito nas linguagens C, C++ ou Java.

####Entrada: Nome do arquivo "Expressoes.in"

Contem várias expressões que serão avaliadas.
A 1ª linha da entrada será um número n, indicando quantas expressões precisarão ter suas entradas avaliadas. Logo a seguir teremos n linhas, onde cada uma possuirá uma expressão, sem espaçamento, que será a expressão a ser avaliada, conforme as regras explicitadas anteriormente. É garantido que todas as expressões são bem-formadas e, consequentemente, possuem resposta e que cada expressão possuirá menos que 1000 caracteres. O aluno que não seguir as especificações dadas perderá 0,5 durante a correção, portanto prestem atenção aos nomes dos arquivos!!!

####Saída: Nome do arquivo "Expressoes.out"

Para cada caso de teste imprima uma linha contendo "caso #x", onde x indica o número de caso de teste, iniciando de '1'. Na mesma linha, imprima a resposta como "satisfativel" ou "insatisfativel". Se a entrada não estiver na Fórmula Normal Conjuntiva, imprima "nao esta na FNC" e passe para a próxima expressão. Se a entrada não estiver de modo que todas as cláusulas são de Horn, imprima "nem todas as clausulas sao de horn" e passe para a próxima expressão. 

Obs:Todo o projeto deve ser entregue em uma única classe. 

```

[Victor Aurélio]: <http://victoraurelio.com>
[Victor Aurélio at CIn]: <http://cin.ufpe.br/~vags>
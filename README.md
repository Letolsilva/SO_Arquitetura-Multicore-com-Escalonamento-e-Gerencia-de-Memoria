<h1 align="center" font-size="200em"><b>Simulador da Arquitetura Multicore de Von Neumann com Suporte a Preempção, Escalonador, cache e gerenciamento de memória</b></h1>

<div align = "center" >
<!-- imagem -->

[![requirement](https://img.shields.io/badge/IDE-Visual%20Studio%20Code-informational)](https://code.visualstudio.com/docs/?dv=linux64_deb)
![Make](https://img.shields.io/badge/Compilacao-Make-orange)
![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-blue)

</div>

## Abstração da Arquitetura 

Confira abaixo o diagrama que ilustra a abstração da arquitetura multicore com escalonadores, em conjunto com a CPU, o sistema operacional, cache e a memória.

![WhatsApp Image 2025-01-31 at 22 37 13](https://github.com/user-attachments/assets/aae5ec1d-7e00-4962-b362-2244d9aa5753)

<!-- <div> 
    Acesse o <a href="https://www.overleaf.com/read/jygddzvfxprd#bbeb2d" target="_blank" rel="noopener noreferrer">Artigo</a> com toda a documentação clicando no link.
</div> -->

## Escalonador
Foram implementados três escalonadores: First Come, First Served (FCFS), Shortest Job Next (SJRN) e Escalonamento por Prioridade. Lembre-se de escolher o escalonador que deseja executar no terminal, utilizando os números 1, 2 ou 3.

![alt text](image.png)
   
## Arquivos de entrada e saída

### Input 

**Observação:** Os arquivos de input devem obrigatoriamente estar no formato **.data**.  
É possível alterar essa configuração no código antes da execução. Certifique-se de armazená-los na pasta **data**.

![image](https://github.com/user-attachments/assets/2c4ebfbc-8dfe-48d7-86f8-203b16c8ec9b)



### Output

O arquivo de saída é armazenado no diretório **output** após a execução do escalonamento Shortest Remaining Job First. Certifique-se de que a pasta **output** tenha sido criada para que o arquivo de saída possa ser gerado. O comando `make` já cria essa pasta automaticamente.

![image](https://github.com/user-attachments/assets/283d21fc-d9a5-4977-9ed7-0133bc7cbeac)


## 👾Compilação e execução

- Especificações da máquina em que o código foi rodado:
  - Processador Intel Core i7, 12th Gen;
  - Sistema Operacional Ubuntu 22.04.5;
  - 16GB de RAM.
- | Comando      | Função                                                                                                                          |
  | ------------ | ---------------------------------------------------------------------------------------                                         |
  | `make clean` | Apaga a última compilação realizada contida na pasta build e o arquivo output                                                   |
  | `make`       | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build e cria uma pasta output caso nao exista |
  | `make run`   | Executa o programa da pasta build após a realização da compilação                                                               |
   
## Contato

<div>
 <br><p align="justify"> Letícia de Oliveira</p>
 <a href="https://t.me/letolsilva">
 <img align="center" src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white"/> 
 </div>
<a style="color:black" href="mailto:letolsilva22@gmail.com?subject=[GitHub]%20Source%20Dynamic%20Lists">
✉️ <i>letolsilva22@gmail.com</i>
</a>

<div>
 <br><p align="justify"> Lucas Lima de Oliveira</p>
 <a href="https://t.me/">
 <img align="center" src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white"/> 
 </div>
<a style="color:black" href="mailto:lucaslimadeoliveira80@gmail.com?subject=[GitHub]%20Source%20Dynamic%20Lists">
✉️ <i>lucaslimadeoliveira80@gmail.com</i>
</a>

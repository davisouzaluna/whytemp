# API de comunicacao com o BD

A API envia os dados para o banco Myqsl, inicialmente utilizando o projeto [MQTTAPI](https://github.com/davisouzaluna/mqttapi)

## Fluxo da aplicação:

1.  `API` ------*Subscribe in humidity and Temperature Topic*---> `Broker`

2. `IOT` ------*Publish humidity and Temperature in Topic*-----> `Broker`

3. `API` ------*Send message to BD* -----------------------------> `MySQL`

## Testes de envio de dados

O arquivo [pub_simulation.py](./pub_simulation.py) simula os dados que o dispositivo IoT deve enviar. Deve ser usado somente para Debug. *A posteriori* será *legado*.


## Requerimentos

- Versão mínima do python 3.11 ou superior
- [Pip](https://pypi.org/project/pip/). Caso não tenha instalado:
    
    ```bash
    sudo apt install python3-pip
    ```
    Para atualizar o pip:
    ```
    python3 -m pip install --upgrade pip
    ```
- Mysql versão 8 ou superior e emqx broker. Veja como [subir os containers](../whytemp_container/README.md)
## Como executar

Depois de [subir os containers](../whytemp_container/README.md), instale as bibliotecas recomendadas

```bash
pip install -r requirements.txt
```

### Autor: Davi Souza de Luna


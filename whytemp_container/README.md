# Containers para a depuração da aplicação

Para permitir o desenvolvimento em diversos ambientes, foi utilizado a ferramenta [docker](https://www.docker.com/) para a instalação de serviços essenciais para a aplicação, evitando o problema de não execução da aplicação em decorrência do ambiente de execução incompatível.

## Serviços utilizados

- [EMQX](https://docs.emqx.com/en/emqx/latest/deploy/install-docker.html)
- [MYSQL](https://hub.docker.com/_/mysql)

## Como subir os containers 

Para subir os serviços, dentro desse diretório e com o docker e docker compose instalados, execute:

```bash
docker-compose up -d
```

Para executar iterativamente cada serviço:

- EMQX

    Se tiver um shell interativo
    ```bash
    docker exec -it emqx /bin/bash
    ```
    Se não tiver um shell interativo
    ```bash
    docker exec -it emqx /bin/sh
    ```
- Mysql
    Sabendo do usuário(nesse caso colocarei o usuário como `user`) e a senha(nesse caso colocarei a senha como `pass`) que você quer se conectar, digite:
    ```bash
    docker exec -it mysql mysql -u user -p 
    ```
    E a senha será requisitada para acessar

## Referências:

-  [Documentação do Docker](https://docs.docker.com/guides/)
-  [Documentação do EMQX](https://docs.emqx.com/en/emqx/latest/)
- ANDERSON, Charles. Docker [software engineering]. Ieee Software, v. 32, n. 3, p. 102-c3, 2015.

### Autor: Davi Souza de Luna




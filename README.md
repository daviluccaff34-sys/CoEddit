# CoEddit

**CoEddit** é um mod para Geometry Dash (Geode) que está sendo desenvolvido para edição colaborativa de fases pela internet.

## Lançamento

Esta versão adiciona a tela de autorização dentro do mod e conecta as ações de criar e procurar salas à API do CoEddit.

## Login / autorização

1. Instale o arquivo \`.geode\` pela Geode.
2. No Geometry Dash, abra o editor para criar uma sala ou a tela de navegador para entrar em uma.
3. Na primeira utilização, o CoEddit solicitará a chave de autorização.
4. Use a chave gratuita abaixo:

\`\`\`text
AUTH000100300401036393972737383737
\`\`\`

A chave é compartilhada e pública. Ela não é uma senha pessoal nem identifica uma conta. Não existe cadastro de usuário.

## Salas

- **HOST / CRIAR SALA:** solicita um nome e cria uma sala online.
- **JOIN / ENTRAR:** solicita o código de uma sala existente.
- O código retornado pelo servidor deve ser compartilhado com os amigos.

## Idioma

Abra as configurações do CoEddit no Geode e escolha **English** ou **Português**.

## Compatibilidade

- Geometry Dash 2.2081
- Geode SDK 5.11.2
- Android 64-bit

## Estado do projeto

A autorização e as chamadas de criação/entrada de sala estão sendo conectadas. A sincronização ao vivo dos objetos do editor e a presença dos participantes ainda estão em desenvolvimento; criar ou entrar em uma sala não significa que a edição já esteja sincronizada.

## Compilação

O GitHub Actions compila o pacote Android64. Acesse a aba **Actions** do repositório para acompanhar a compilação.

## Download

Os pacotes oficiais do projeto serão publicados em **GitHub Releases**, na versão identificada como **lançamento**.

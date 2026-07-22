@echo off
title Sincronizador Git - sovcw-openbor-project
color 0A

echo ===========================================
echo    Sincronizando Repositorio OpenBOR
echo ===========================================
echo.

:: Garante que o script rode na pasta correta
cd /d "D:\Felipe\OB\Streets of Vendetta CracolandWar(2026)\Data"

echo [1/4] Adicionando arquivos alterados...
git add .

echo.
set /p msg="Digite a mensagem do commit (ou ENTER para mensagem automatica): "

if "%msg%"=="" (
    set msg=Update %date% %time:~0,5%
)

echo.
echo [2/4] Criando commit com a mensagem: "%msg%"...
git commit -m "%msg%"

echo.
echo [3/4] Baixando atualizacoes do repositorio remoto (Pull)...
git pull origin main --rebase

echo.
echo [4/4] Enviando alteracoes para o GitHub (Push)...
git push -u origin main

echo.
echo ===========================================
echo    Sincronizacao concluida com sucesso!
echo ===========================================
echo.
pause
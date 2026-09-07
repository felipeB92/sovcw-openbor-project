void bindSpawn(void vName, float fX, float fY, float fZ)
{
    void self = getlocalvar("self");
    void vSpawn;
    int dir = getentityproperty(self, "direction");

    // 1. Limpa o cache de spawn da engine para evitar conflitos com outros personagens
    clearspawnentry();
    
    // 2. Define o nome da entidade que será criada (ex: "Burn01")
    setspawnentry("name", vName);
    
    // 3. Cria a entidade usando a função nativa universal do OpenBOR
    vSpawn = spawn();

    // 4. Verifica se a entidade foi criada com sucesso antes de tentar grudá-la
    if(vSpawn != NULL())
    {
        // Se o Galsia estiver virado para a esquerda (0), inverte a posição X do fogo
        if(dir == 0){
            fX = -fX;
        }

        // 5. Prende o fogo no Galsia
        // Ordem nativa: bindentity(filho, pai, x, z, y, direcao, trava_animacao)
        bindentity(vSpawn, self, fX, fZ, fY, dir, 0);
    }
}
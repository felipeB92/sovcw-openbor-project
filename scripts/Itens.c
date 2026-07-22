void detector_item(int saude)
{
    void self = getlocalvar("self");
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float y = getentityproperty(self, "y");
    
    int i;

    float tol_x = 15; 
    float tol_z = 10; 
    float tol_y = 1; 

    for(i = 0; i < 4; i++)
    {
        void player = getplayerproperty(i, "entity");

        if(player != NULL())
        {
            float px = getentityproperty(player, "x");
            float pz = getentityproperty(player, "z");
            float py = getentityproperty(player, "y");

            if(px >= x - tol_x && px <= x + tol_x && 
               pz >= z - tol_z && pz <= z + tol_z && 
               py >= y - tol_y && py <= y + tol_y)
            {
                int keys = getplayerproperty(i, "keys");

                if(keys & openborconstant("FLAG_ATTACK"))
                {
                    int hp = getentityproperty(player, "health");
                    int max_hp = getentityproperty(player, "maxhealth");

                    if(hp < max_hp)
                    {
                        // MUDANÇA AQUI: Em vez de ANI_GET, chamamos o FOLLOW10
                        // O número 10 aqui deve ser o mesmo que você colocou no personagem
                        executeanimation(player, openborconstant("ANI_FOLLOW10"));

                        // Aplica a cura
                        int nova_vida = hp + saude;
                        if(nova_vida > max_hp) nova_vida = max_hp;
                        changeentityproperty(player, "health", nova_vida);
                        
                        // Remove a maçã
                        killentity(self);
                    }
                }
            }
        }
    }
}

void detector_gold(int pontos)
{
    void self = getlocalvar("self");
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float y = getentityproperty(self, "y");
    int i;

    float tol_x = 15; 
    float tol_z = 10; 
    float tol_y = 2; 

    for(i = 0; i < 4; i++)
    {
        void player = getplayerproperty(i, "entity");

        if(player != NULL())
        {
            float px = getentityproperty(player, "x");
            float pz = getentityproperty(player, "z");
            float py = getentityproperty(player, "y");

            if(px >= x - tol_x && px <= x + tol_x && 
               pz >= z - tol_z && pz <= z + tol_z && 
               py >= y - tol_y && py <= y + tol_y)
            {
                int keys = getplayerproperty(i, "keys");

                if(keys & openborconstant("FLAG_ATTACK"))
                {
                    // Faz o personagem usar a animação com o som de Moeda
                    executeanimation(player, openborconstant("ANI_FOLLOW11"));

                    // Adiciona a pontuação (Score)
                    int p_index = getentityproperty(player, "playerindex");
                    int score_atual = getplayerproperty(p_index, "score");
                    changeplayerproperty(p_index, "score", score_atual + pontos);
                    
                    killentity(self);
                    return;
                }
            }
        }
    }
}
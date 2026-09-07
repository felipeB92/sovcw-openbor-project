#include "data/scripts/player.c"
void quebrar_arma(int chance)
{
  // --- 1-in-chance chance of weapon breaking after being used
  if (rand() % chance == 0) {
    
     void vSelf = getlocalvar("self"); //Get calling entity.
     int iMap = getentityproperty(vSelf, "map"); //Get current remap.
     int iWep = 0;     //weapon index 0 == unarmed?

     if ((iWep) && (iMap)){ //If requested model is not default and a remap is being used, record remap.
        setglobalvar("iMap" + vSelf, iMap); 
     } 
     
changeentityproperty(vSelf, "weapon", iWep, 0); //Switch to desired model.
     changeentityproperty(vSelf, "map", getglobalvar("iMap" + vSelf)); //Defeat weapon remap bug by forcing a switch back to remap.
  }    
}
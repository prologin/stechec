<?php 
$race_tbt = array();
$race_tbt["---"] = "";
$race_tbt["Amazones"]="amazons";
$race_tbt["Chaos"]="chaos";
$race_tbt["Elfes"]="elves";
$race_tbt["Elfes Noirs"]="darkelves";
$race_tbt["Elfes Sylvains"]="woodelves";
$race_tbt["Goblins"]="goblins";
$race_tbt["Halflings"]="halflings";
$race_tbt["Hauts Elfes"]="highelves";
$race_tbt["Hommes Lezards"]="lizardmen";
$race_tbt["Humains"]="humans";
$race_tbt["Khemris"]="khemris";
$race_tbt["Morts-Vivants"]="undeads";
$race_tbt["Nains Du Chaos"]="chaosdwarves";
$race_tbt["Nains"]="dwarves";
$race_tbt["Necromantiques"]="necromantics";
$race_tbt["Nordiques"]="norses";
$race_tbt["Ogres"]="ogres";
$race_tbt["Orques"]="orc";
$race_tbt["Pourris de Nurgle"]="nurgles";
$race_tbt["Skavens"]="skavens";
$race_tbt["Vampires"]="vampires";

$race_singulier_tbt = array();
$race_singulier_tbt["---"] = "";
$race_singulier_tbt["Amazones"]="amazon";
$race_singulier_tbt["Chaos"]="chaos";
$race_singulier_tbt["Elfes"]="elf";
$race_singulier_tbt["Elfes Noirs"]="darkelf";
$race_singulier_tbt["Elfes Sylvains"]="woodelf";
$race_singulier_tbt["Goblins"]="goblin";
$race_singulier_tbt["Halflings"]="halfling";
$race_singulier_tbt["Hauts Elfes"]="highelf";
$race_singulier_tbt["Hommes Lezards"]="lizardman";
$race_singulier_tbt["Humains"]="human";
$race_singulier_tbt["Khemris"]="khemri";
$race_singulier_tbt["Morts-Vivants"]="undead";
$race_singulier_tbt["Nains Du Chaos"]="chaosdwarf";
$race_singulier_tbt["Nains"]="dwarf";
$race_singulier_tbt["Necromantiques"]="necromantics";
$race_singulier_tbt["Nordiques"]="norse";
$race_singulier_tbt["Ogres"]="ogre";
$race_singulier_tbt["Orques"]="orc";
$race_singulier_tbt["Pourri de Nurgle"]="nurgle";
$race_singulier_tbt["Skavens"]="skaven";
$race_singulier_tbt["Vampires"]="vampire";

$positionid = array();
$display = array();

$positionid["amazon"]["Trois Quart"] = "3";
$positionid["amazon"]["Lanceuse"] = "2";
$positionid["amazon"]["Coureuse"] = "1";
$positionid["amazon"]["Receveuse"] = "0";

$positionid["chaos"]["Homme-Bete"] = "1";
$positionid["chaos"]["Guerrier du Chaos"] = "0";

$positionid["chaosdwarf"]["Hobgoblin"] = "1";
$positionid["chaosdwarf"]["Bloqueur"] = "0";
$positionid["chaosdwarf"]["Centaure"] = "2";

$positionid["darkelf"]["Trois Quart"] = "3";
$positionid["darkelf"]["Lanceur"] = "2";
$positionid["darkelf"]["Coureur"] = "1";
$positionid["darkelf"]["Furie"] = "0";

$positionid["dwarf"]["Longue-Barbe"] = "3";
$positionid["dwarf"]["Sprinter"] = "2";
$positionid["dwarf"]["Coureur"] = "1";
$positionid["dwarf"]["Tueur de Trolls"] = "0";

$positionid["elf"]["Trois Quart"]="3";
$positionid["elf"]["Lanceur"]="2";
$positionid["elf"]["Receveur"]="1";
$positionid["elf"]["Coureur"]="0";

$positionid["goblin"]["Gobelin"] = "0";

$positionid["halfling"]["Halfling"] = "0";

$positionid["highelf"]["Trois Quart"] = "3";
$positionid["highelf"]["Guerrier Phoenix"] = "1";
$positionid["highelf"]["Guerrier Lion"] = "2";
$positionid["highelf"]["Guerrier Dragon"] = "0";

$positionid["lizardman"]["Skink"] = "1";
$positionid["lizardman"]["Saurus"] = "0";

$positionid["human"]["Trois Quart"] = "3";
$positionid["human"]["Receveur"] = "2";
$positionid["human"]["Lanceur"] = "1";
$positionid["human"]["Receveur"] = "0";

$positionid["khemri"]["Squelette"]="3";
$positionid["khemri"]["Thro-Ra"]="2";
$positionid["khemri"]["Blitz-Ra"]="1";
$positionid["khemri"]["Momies"]="0";

$positionid["norse"]["Trois Quart"] = "3";
$positionid["norse"]["Réceptionneur"] = "2";
$positionid["norse"]["Lanceur"] = "1";
$positionid["norse"]["Coureur"] = "0";

$positionid["necromantics"]["Loup Garou"] = "4";
$positionid["necromantics"]["Zombie"] = "3";
$positionid["necromantics"]["Goule"] = "2";
$positionid["necromantics"]["Revenant"] = "1";
$positionid["necromantics"]["Golem de Chair"] = "0";

$positionid["nurgle"]["Trois Quart"] = "1";
$positionid["nurgle"]["Réceptionneur"] = "0";

$positionid["ogre"]["Gobelin"] = "1";
$positionid["ogre"]["Ogre"] = "0";

$positionid["orc"]["Trois Quart"] = "3";
$positionid["orc"]["Lanceur"] = "1";
$positionid["orc"]["Orque Noir"] = "2";
$positionid["orc"]["Coureur"] = "0";

$positionid["skaven"]["Trois Quart"] = "3";
$positionid["skaven"]["Lanceur"] = "1";
$positionid["skaven"]["Coureur Egouts"] = "2";
$positionid["skaven"]["Vermine de Choc"] = "0";

$positionid["undead"]["Squelette"] = "4";
$positionid["undead"]["Zombie"] = "3";
$positionid["undead"]["Goule"] = "2";
$positionid["undead"]["Revenant"] = "1";
$positionid["undead"]["Momie"] = "0";

$positionid["vampire"]["Serviteur"] = "1";
$positionid["vampire"]["Vampire"] = "0";

$positionid["woodelf"]["Trois Quart"] = "3";
$positionid["woodelf"]["Receveur"] = "1";
$positionid["woodelf"]["Lanceur"] = "2";
$positionid["woodelf"]["Danseur de Guerre"] = "0";


$display["amazon"]["Trois Quart"] = "amazon_l.png";
$display["amazon"]["Lanceuse"] = "amazon_t.png";
$display["amazon"]["Coureuse"] = "amazon_b.png";
$display["amazon"]["Receveuse"] = "amazon_c.png";

$display["chaos"]["Homme-Bete"] = "chaos_b.png";
$display["chaos"]["Guerrier du Chaos"] = "chaos_w.png";

$display["chaosdwarf"]["Hobgoblin"] = "chaosdwarf_h.png";
$display["chaosdwarf"]["Bloqueur"] = "chaosdwarf_b.png";
$display["chaosdwarf"]["Centaure"] = "chaosdwarf_.png";

$display["darkelf"]["Trois Quart"] = "darkelf_l.png";
$display["darkelf"]["Lanceur"] = "darkelf_t.png";
$display["darkelf"]["Coureur"] = "darkelf_b.png";
$display["darkelf"]["Furie"] = "darkelf_w.png";

$display["dwarf"]["Longue-Barbe"] = "dwarf_lb.png";
$display["dwarf"]["Sprinter"] = "dwarf_r.png";
$display["dwarf"]["Coureur"] = "dwarf_b.png";
$display["dwarf"]["Tueur de Trolls"] = "dwarf_ts.png";

$display["elf"]["Trois Quart"]="elf_l.png";
$display["elf"]["Lanceur"]="elf_t.png";
$display["elf"]["Receveur"]="elf_c.png";
$display["elf"]["Coureur"]="elf_b.png";

$display["goblin"]["Gobelin"] = "goblin_g.png";

$display["halfling"]["Halfling"] = "halfling_h.png";

$display["highelf"]["Trois Quart"] = "highelf_l.png";
$display["highelf"]["Guerrier Phoenix"] = "highelf_pw.png";
$display["highelf"]["Guerrier Lion"] = "highelf_lw.png";
$display["highelf"]["Guerrier Dragon"] = "highelf_dw.png";

$positionid["lizardman"]["Skink"] = "lizardmen_sk.png";
$positionid["lizardman"]["Saurus"] = "lizardmen_sa.png";

$display["human"]["Trois Quart"] = "human_l.png";
$display["human"]["Receveur"] = "human_c.png";
$display["human"]["Lanceur"] = "human_t.png";
$display["human"]["Coureur"] = "human_b.png";

$display["khemri"]["Squelette"]="khemri_s.png";
$display["khemri"]["Thro-Ra"]="khemri_t.png";
$display["khemri"]["Blitz-Ra"]="khemri_b.png";
$display["khemri"]["Momies"]="khemri_m.png";

$display["norse"]["Trois Quart"] = "norse_l.png";
$display["norse"]["Réceptionneur"] = "norse_c.png";
$display["norse"]["Lanceur"] = "norse_t.png";
$display["norse"]["Coureur"] = "norse_b.png";

$display["necromantics"]["Loup Garou"] = "necromantic_W.png";
$display["necromantics"]["Zombie"] = "necromantic_z.png";
$display["necromantics"]["Goule"] = "necromantic_g.png";
$display["necromantics"]["Revenant"] = "necromantic_w.png";
$display["necromantics"]["Golem de Chair"] = "necromantic_f.png";

$display["nurgle"]["Homme Bête de Nurgle"] = "nurgle_b.png";
$display["nurgle"]["Pourri"] = "nurgle_r.png";

$display["orc"]["Trois Quart"] = "orc_l.png";
$display["orc"]["Lanceur"] = "orc_t.png";
$display["orc"]["Orque Noir"] = "orc_bo.png";
$display["orc"]["Coureur"] = "orc_b.png";

$display["ogre"]["Gobelin"] = "goblin_g.png";
$display["ogre"]["Ogre"] = "ogre_o.png";

$display["skaven"]["Trois Quart"] = "skaven_l.png";
$display["skaven"]["Lanceur"] = "skaven_t.png";
$display["skaven"]["Coureur Egouts"] = "skaven_gr.png";
$display["skaven"]["Vermine de Choc"] = "skaven_sv.png";

$display["undead"]["Squelette"] = "undead_s.png";
$display["undead"]["Zombie"] = "undead_z.png";
$display["undead"]["Goule"] = "undead_g.png";
$display["undead"]["Revenant"] = "undead_w.png";
$display["undead"]["Momie"] = "undead_m.png";

$positionid["vampire"]["Serviteur"] = "vampire_s.png";
$positionid["vampire"]["Vampire"] = "vampire_v.png";

$display["woodelf"]["Trois Quart"] = "woodelf_l.png";
$display["woodelf"]["Receveur"] = "woodelf_c.png";
$display["woodelf"]["Lanceur"] = "woodelf_p.png";
$display["woodelf"]["Danseur de Guerre"] = "woodelf_w.png";

?>
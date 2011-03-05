<?php 
$race_tbt = array();
$race_tbt["---"] = "";
$race_tbt["Amazone"] = "";
$race_tbt["Chaos"]="chaos";
$race_tbt["Dark Elf"]="darkelves";
$race_tbt["Wood Elf"]="woodelves";
$race_tbt["Goblin"]="goblins";
$race_tbt["Halfling"]="halflings";
$race_tbt["High Elfes"]="highelves";
$race_tbt["Lizardman"]="";
$race_tbt["Human"]="humans";
$race_tbt["Undead"]="undeads";
$race_tbt["Chaos Dwarf"]="chaosdwarves";
$race_tbt["Dwarf"]="dwarves";
$race_tbt["Norse"]="norses";
$race_tbt["Orc"]="orc";
$race_tbt["Skaven"]="skavens";

$race_singulier_tbt = array();
$race_singulier_tbt["---"] = "";
$race_singulier_tbt["Amazone"] = "";
$race_singulier_tbt["Chaos"]="chaos";
$race_singulier_tbt["Dark Elf"]="darkelf";
$race_singulier_tbt["Wood Elf"]="woodelf";
$race_singulier_tbt["Goblin"]="goblin";
$race_singulier_tbt["Halfling"]="halfling";
$race_singulier_tbt["High Elfes"]="highelf";
$race_singulier_tbt["Lizardmen"]="";
$race_singulier_tbt["Human"]="human";
$race_singulier_tbt["Undead"]="undead";
$race_singulier_tbt["Chaos Dwarf"]="chaosdwarf";
$race_singulier_tbt["Dwarf"]="dwarf";
$race_singulier_tbt["Norse"]="norse";
$race_singulier_tbt["Orc"]="orc";
$race_singulier_tbt["Skaven"]="skaven";

$positionid = array();
$display = array();

$positionid["chaos"]["Beastman"] = "1";
$positionid["chaos"]["Chaos Warrior"] = "0";

$positionid["chaosdwarf"]["Hobgoblin"] = "1";
$positionid["chaosdwarf"]["Blocker"] = "0";
$positionid["chaosdwarf"]["Bull Centaur"] = "2";

$positionid["darkelf"]["Lineman"] = "3";
$positionid["darkelf"]["Thrower"] = "2";
$positionid["darkelf"]["Blitzer"] = "1";
$positionid["darkelf"]["Witch Elf"] = "0";

$positionid["dwarf"]["Long Beard"] = "3";
$positionid["dwarf"]["Runner"] = "2";
$positionid["dwarf"]["Blitzer"] = "1";
$positionid["dwarf"]["Troll Killer"] = "0";

$positionid["goblin"]["Goblin"] = "0";

$positionid["halfling"]["Halfling"] = "0";

$positionid["highelf"]["Lineman"] = "3";
$positionid["highelf"]["Phoenix Warrior"] = "1";
$positionid["highelf"]["Lion Warrior"] = "2";
$positionid["highelf"]["Dragon Warrior"] = "0";

$positionid["human"]["Lineman"] = "3";
$positionid["human"]["Catcher"] = "2";
$positionid["human"]["Thrower"] = "1";
$positionid["human"]["Blitzer"] = "0";

$positionid["norse"]["Lineman"] = "3";
$positionid["norse"]["Catcher"] = "2";
$positionid["norse"]["Thrower"] = "1";
$positionid["norse"]["Blitzer"] = "0";

$positionid["orc"]["Lineman"] = "3";
$positionid["orc"]["Thrower"] = "1";
$positionid["orc"]["Black Orc"] = "2";
$positionid["orc"]["Blitzer"] = "0";

$positionid["skaven"]["Lineman"] = "3";
$positionid["skaven"]["Thrower"] = "1";
$positionid["skaven"]["Gutter Runner"] = "2";
$positionid["skaven"]["Storm Vermine"] = "0";

$positionid["undead"]["Squeleton"] = "4";
$positionid["undead"]["Zombie"] = "3";
$positionid["undead"]["Goul"] = "2";
$positionid["undead"]["Whight"] = "1";
$positionid["undead"]["Mummie"] = "0";

$positionid["woodelf"]["Lineman"] = "3";
$positionid["woodelf"]["Catcher"] = "1";
$positionid["woodelf"]["Thrower"] = "2";
$positionid["woodelf"]["Wardancer"] = "0";



$display["chaos"]["Beastman"] = "chaos_b.png";
$display["chaos"]["Chaos Warrior"] = "chaos_w.png";

$display["chaosdwarf"]["Hobgoblin"] = "chaosdwarf_h.png";
$display["chaosdwarf"]["Blocker"] = "chaosdwarf_b.png";
$display["chaosdwarf"]["Bull Centaur"] = "chaosdwarf_.png";

$display["darkelf"]["Lineman"] = "darkelf_l.png";
$display["darkelf"]["Thrower"] = "darkelf_t.png";
$display["darkelf"]["Blitzer"] = "darkelf_b.png";
$display["darkelf"]["Witch Elf"] = "darkelf_w.png";

$display["dwarf"]["Long Beard"] = "dwarf_lb.png";
$display["dwarf"]["Runner"] = "dwarf_r.png";
$display["dwarf"]["Blitzer"] = "dwarf_b.png";
$display["dwarf"]["Troll Slayer"] = "dwarf_ts.png";

$display["goblin"]["Goblin"] = "goblin_g.png";

$display["halfling"]["Halfling"] = "halfling_h.png";

$display["highelf"]["Lineman"] = "highelf_l.png";
$display["highelf"]["Phoenix Warrior"] = "highelf_pw.png";
$display["highelf"]["Lion Warrior"] = "highelf_lw.png";
$display["highelf"]["Dragon Warrior"] = "highelf_dw.png";

$display["human"]["Lineman"] = "human_l.png";
$display["human"]["Catcher"] = "human_c.png";
$display["human"]["Thrower"] = "human_t.png";
$display["human"]["Blitzer"] = "human_b.png";

$display["norse"]["Lineman"] = "norse_l.png";
$display["norse"]["Catcher"] = "norse_c.png";
$display["norse"]["Thrower"] = "norse_t.png";
$display["norse"]["Blitzer"] = "norse_b.png";

$display["orc"]["Lineman"] = "orc_l.png";
$display["orc"]["Thrower"] = "orc_t.png";
$display["orc"]["Black Orc"] = "orc_bo.png";
$display["orc"]["Blitzer"] = "orc_b.png";

$display["skaven"]["Lineman"] = "skaven_l.png";
$display["skaven"]["Thrower"] = "skaven_t.png";
$display["skaven"]["Gutter Runner"] = "skaven_gr.png";
$display["skaven"]["Storm Vermine"] = "skaven_sv.png";

$display["undead"]["Squeleton"] = "undead_s.png";
$display["undead"]["Zombie"] = "undead_z.png";
$display["undead"]["Goul"] = "undead_g.png";
$display["undead"]["Whight"] = "undead_w.png";
$display["undead"]["Mummie"] = "undead_m.png";

$display["woodelf"]["Lineman"] = "woodelf_l.png";
$display["woodelf"]["Catcher"] = "woodelf_c.png";
$display["woodelf"]["Thrower"] = "woodelf_p.png";
$display["woodelf"]["Wardancer"] = "woodelf_w.png";

?>
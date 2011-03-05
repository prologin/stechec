var ns=(document.layers)?1:0;
var ie=document.all&&navigator.userAgent.indexOf("Opera")==-1

function popup_embleme(emb){
	var emb_win = window.open('emblem_loader.php3?emb='+emb,'emb_win','toolbar=no,status=no,scrollbars=no,location=no,menubar=no,directories=no,width=250,height=350');
	emb_win.focus();
}

function changer_embleme(emb){
	document.feuille.emb_img.src = "../emblems/"+emb;
	if(ie) emb_name.innerHTML = emb;
	else if(ns) document.emb_name.innerHTML = emb;
	else document.getElementById("emb_name").innerHTML = emb;

	document.feuille.emb.value = emb;
}

function editer(id){
	document.feuille.editable.value = id;
	document.feuille.submit();
}

function calculer_xps(j){
	eval('var position_ok = document.feuille.p'+j+'.options[document.feuille.p' + j + '.selectedIndex].text;');
	if(position_ok!="---" ){	
		eval('jpv_val = document.feuille.jpv'+j+'.selectedIndex');
		eval('td_val = document.feuille.td'+j+'.selectedIndex');
		eval('int_val = document.feuille.int'+j+'.selectedIndex');
		eval('bl_val = document.feuille.bl'+j+'.selectedIndex');
		eval('reu_val = document.feuille.reu'+j+'.selectedIndex');
		eval('sor_val = document.feuille.sor'+j+'.selectedIndex');
		xps = 5*jpv_val + 3*td_val + 2*(int_val + bl_val+sor_val) + reu_val;
		eval('document.feuille.xp'+j+'.value = '+xps);
		calculer_total_equipe();
	}
}

function calculer_total_equipe(){
	
	document.feuille.cout_rel.value = document.feuille.x_rel.value * document.feuille.rel.selectedIndex;
	document.feuille.cout_pop.value = 10000 * document.feuille.pop.selectedIndex;
	document.feuille.cout_ass.value = 10000 * document.feuille.ass.selectedIndex;
	document.feuille.cout_ppg.value = 10000 * document.feuille.ppg.selectedIndex;
	document.feuille.cout_apo.value = 50000 * document.feuille.apo.selectedIndex;

	total = 0;
	total += Math.round(document.feuille.cout1.value); 
	total += Math.round(document.feuille.cout2.value);
	total += Math.round(document.feuille.cout3.value); 
	total += Math.round(document.feuille.cout4.value);
	total += Math.round(document.feuille.cout5.value);
	total += Math.round(document.feuille.cout6.value);
	total += Math.round(document.feuille.cout7.value);
	total += Math.round(document.feuille.cout8.value);
	total += Math.round(document.feuille.cout9.value);
	total += Math.round(document.feuille.cout10.value);
	total += Math.round(document.feuille.cout11.value);
	total += Math.round(document.feuille.cout12.value);
	total += Math.round(document.feuille.cout13.value);
	total += Math.round(document.feuille.cout14.value);
	total += Math.round(document.feuille.cout15.value);
	total += Math.round(document.feuille.cout16.value);
	total += Math.round(document.feuille.cout_rel.value);
	total += Math.round(document.feuille.cout_pop.value);
	total += Math.round(document.feuille.cout_ass.value);
	total += Math.round(document.feuille.cout_ppg.value);
	total += Math.round(document.feuille.cout_apo.value);

	document.feuille.cout_eq.value = total;

	somme_xps = 0;
	for(i=1;i<=16;i++){
		eval('tmp = document.feuille.xp'+i+'.value');
		somme_xps = somme_xps + Math.floor(tmp);
	}
	classement = document.feuille.cout_eq.value/10000 + Math.floor(somme_xps/5);
	document.feuille.cla.value = classement;
}

function choix_race(){
	if( document.feuille.race.selectedIndex==0 ) return;
	document.feuille.r.value = document.feuille.race.options[document.feuille.race.selectedIndex].text;
	document.feuille.submit();
}

function change_description_race(){
	var e;
	if(race_tbt[document.feuille.race.options[document.feuille.race.selectedIndex].text]!="") e = race_tbt[document.feuille.race.options[document.feuille.race.selectedIndex].text] + ".jpg";
	else e = "no_emblem.gif";

	document.feuille.desc_emb_race.src = "../emblems/"+e;

//	img_race.innerHTML = "<img src='emblems/"+e+"' width='70' height='93' border='1' bordercolor='#DDDDDD'>";
	if( description_race_en[document.feuille.race.options[document.feuille.race.selectedIndex].text]=='') {
		if(ie) {
			window.status = "IE version";
			texte_race.innerHTML="---";	
		}
		else if(ns) {
			window.status = "NS version";
			document.texte_race.innerHTML="---";
		}
		else {
			window.status = "Default version";
			document.getElementById("texte_race").innerHTML = "---";
		}
	}
	else {
		if(ie) {
			window.status = "IE version";
			texte_race.innerHTML = description_race_en[document.feuille.race.options[document.feuille.race.selectedIndex].text]; 
		}
		else if(ns) {
			window.status = "NS version";
			document.texte_race.innerHTML = description_race_fr[document.feuille.race.options[document.feuille.race.selectedIndex].text] + "<br>---<br>" + description_race_en[document.feuille.race.options[document.feuille.race.selectedIndex].text]; 
		}
		else {
			window.status = "Default version";
			document.getElementById("texte_race").innerHTML = description_race_fr[document.feuille.race.options[document.feuille.race.selectedIndex].text] + "<br>---<br>" + description_race_en[document.feuille.race.options[document.feuille.race.selectedIndex].text]; 
		}

	}
}

function reset_value(j){
	eval('document.feuille.ma' + j + '.selectedIndex = 0');
	eval('document.feuille.st' + j + '.selectedIndex = 0');
	eval('document.feuille.ag' + j + ' .selectedIndex = 0');
	eval('document.feuille.av' + j + '.selectedIndex = 0');
	eval('document.feuille.c' + j + '.value = \'\'');
	eval('document.feuille.c_lg' + j + '.value = \'\'');
	eval('document.feuille.cout' + j + '.value = 0');
}

function translate(comp){
	var separator = ',';
	var stringArray = comp.split(separator);
	var trad = "";

	for (var i=0; i < stringArray.length; i++){
		if(i!=0) trad += ",";
		if (stringArray[i]=='') t = '';
		else t = skills_en_to_fr[stringArray[i]] ;
		trad += t;
	}
	return trad;
}

function check_limitation(j){
	var n=0;
	for (i = 0; i < document.feuille.elements.length; i++){
		if ((document.feuille.elements[i].type == "hidden") && 
			(document.feuille.elements[i].name.indexOf('p') == 0) && 
			(document.feuille.elements[i].value==position_name[j])) { 
				n++;
		}
	}

	if(n>=position_limit[j]) 
		return false;
	else 
		return true;
}

function set_default_value(o, j){
	var i = o.selectedIndex;

	if(check_limitation(i)){
		eval('document.feuille.ma' + j + '.selectedIndex = ' + position_ma[i]);
		eval('document.feuille.st' + j + '.selectedIndex = ' + position_st[i]);
		eval('document.feuille.ag' + j + ' .selectedIndex = ' + position_ag[i]);
		eval('document.feuille.av' + j + '.selectedIndex = ' + position_av[i]);
		eval('document.feuille.c' + j + '.value = \'' + position_comp[i] + '\'');
		eval('document.feuille.c_lg' + j + '.value = \'' + translate(position_comp[i]) + '\'');
		eval('document.feuille.cout' + j + '.value = ' + position_cout[i]);	
	}
	else {
		alert("Attention, vous n'avez droit qu'a " + position_limit[i] + " " + position_name[i]+ "(s)");
		o.selectedIndex = 0;
		set_default_value(o, j);
	}

}

function add_skill_popup(numero,race,position,liste_comp){
	eval('var position_ok = document.feuille.p'+numero+'.options[document.feuille.p' + numero + '.selectedIndex].text;');
	if(position_ok!="---"){
		var skill_win = window.open('select_skill.php?r='+race+'&n='+numero+'&p='+position+'&c='+liste_comp,'skill_win','toolbar=no,status=no,scrollbars=no,location=no,menubar=no,directories=no,width=300,height=200');
		skill_win.focus();
	}
	else alert("Merci de choisir la position du joueur...");
}

function generer_xml(){
	if(document.feuille.dl_xml.value == 0) {
		var xml_win = window.open('','xml_win','resizable=yes,toolbar=no,status=no,scrollbars=yes,location=no,menubar=no,directories=no,width=640,height=480');
		document.feuille.target='xml_win';
	}
	document.feuille.action = 'export_xml.php';
	document.feuille.submit();
	if(document.feuille.dl_xml.value == 0) xml_win.focus();
	document.feuille.target = '';
	document.feuille.action = 'roster.php';
}

function version_imprimable(){
	var print_win = window.open('','print_win','resizable=yes,toolbar=no,status=0,scrollbars=no,location=no,menubar=no,directories=no,width=810,height=460');
	document.feuille.target = 'print_win';
	document.feuille.action = 'printable_roster.php';
	document.feuille.submit();
	print_win.focus();
	document.feuille.target = '';
	document.feuille.action = 'roster.php';
}

function open_editeur_formation(){
	var edit_formation_win = window.open('','edit_formation_win','resizable=yes,toolbar=no,status=0,scrollbars=no,location=no,menubar=no,directories=no,width=700,height=460');
	document.feuille.target = 'edit_formation_win';
	document.feuille.action = 'edit_formation.php';
	document.feuille.submit();
	edit_formation_win.focus();
	document.feuille.target = '';
	document.feuille.action = 'roster.php';
}
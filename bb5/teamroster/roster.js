function popup_embleme(emb){
	var emb_win = window.open('emblem_loader.php?emb='+emb,'emb_win','toolbar=no,status=no,scrollbars=no,location=no,menubar=no,directories=no,width=250,height=350');
	emb_win.focus();
}

function changer_embleme(emb){
	document.getElementById('emb_img').src = "emblems/"+emb;
	document.getElementById('emb_img').alt = emb;
	document.getElementById('emb').value = emb;
}

function calculer_xps(j){
	var position_ok = document.getElementById('p'+j).options[document.getElementById('p'+j).selectedIndex].text;
	if(position_ok!="---" ){
		jpv_val = Math.round(document.getElementById('jpv'+j).value);
		td_val = Math.round(document.getElementById('td'+j).value);
		int_val = Math.round(document.getElementById('int'+j).value);
		bl_val = Math.round(document.getElementById('bl'+j).value);
		reu_val = Math.round(document.getElementById('reu'+j).value);
		sor_val = Math.round(document.getElementById('sor'+j).value);
		xps = 5*jpv_val + 3*td_val + 2*(int_val + bl_val + sor_val) + 1*reu_val;
		document.getElementById('xp'+j).innerHTML = xps;
		document.getElementById('xp_v'+j).value = xps;
		calculer_total_equipe();
	}
}

function calculer_total_equipe(){
	document.getElementById('div_skills').style.display = 'none';

	document.getElementById('cout_rel').innerHTML = document.getElementById('x_rel').value * document.getElementById('rel').value;
	document.getElementById('cout_pop').innerHTML = 10000 * document.getElementById('pop').value;
	document.getElementById('cout_ass').innerHTML = 10000 * document.getElementById('ass').value;
	document.getElementById('cout_ppg').innerHTML = 10000 * document.getElementById('ppg').value;
	document.getElementById('cout_apo').innerHTML = 50000 * document.getElementById('apo').value;

	document.getElementById('cout_rel_v').value = document.getElementById('cout_rel').innerHTML;
	document.getElementById('cout_pop_v').value = document.getElementById('cout_pop').innerHTML;
	document.getElementById('cout_ass_v').value = document.getElementById('cout_ass').innerHTML;
	document.getElementById('cout_ppg_v').value = document.getElementById('cout_ppg').innerHTML;
	document.getElementById('cout_apo_v').value = document.getElementById('cout_apo').innerHTML;
	

	total = 0;
	for(i=1;i<=16;i++){
		total += Math.round(document.getElementById('cout'+i).innerHTML); 
	}

	total += Math.round(document.getElementById('cout_rel').innerHTML);
	total += Math.round(document.getElementById('cout_pop').innerHTML);
	total += Math.round(document.getElementById('cout_ass').innerHTML);
	total += Math.round(document.getElementById('cout_ppg').innerHTML);
	total += Math.round(document.getElementById('cout_apo').innerHTML);

	document.getElementById('cout_eq').innerHTML = total;
	document.getElementById('cout_eq_v').value = total;

	somme_xps = 0;
	for(i=1;i<=16;i++){
		somme_xps += Math.floor(document.getElementById('xp'+i).innerHTML);
	}
	classement = Math.round(document.getElementById('cout_eq').innerHTML)/10000 + Math.floor(somme_xps/5);
	document.getElementById('cla').innerHTML = classement;
	document.getElementById('cla_v').value = classement;
}

function choix_race(){
	if( document.getElementById('race').selectedIndex==0 ) return;
	document.getElementById('r').value = document.getElementById('race').options[document.getElementById('race').selectedIndex].text;
	document.getElementById('feuille').submit();
}

function change_description_race(){
	var e;
	var r = document.getElementById('race').options[document.getElementById('race').selectedIndex].text;
	if(race_tbt[r]!="") 
		e = race_tbt[r] + ".jpg";
	else 
		e = "no_emblem.gif";

	document.getElementById('desc_emb_race').src = "emblems/"+e;

	if( description_race_fr[r]=='' && description_race_en[r]=='' ) {
		window.status = "Tow Bowl - Tactics";
		document.getElementById('texte_race').innerHTML = "---";
	}
	else {
		window.status = "Tow Bowl - Tactics";
		document.getElementById('texte_race').innerHTML = description_race_fr[r] + "<br>---<br>" + description_race_en[r]; 
	}
}

function reset_value(j){
	document.getElementById('div_skills').style.display = 'none';
	document.getElementById('ma'+j).value = '0';
	document.getElementById('st'+j).value = '0';
	document.getElementById('ag'+j).value = '0';
	document.getElementById('av'+j).value = '0';
	document.getElementById('c_lg'+j).innerHTML = '';
	document.getElementById('c'+j).value = '';
	document.getElementById('cout'+j).innerHTML = '0';
	document.getElementById('cout_v'+j).value = '0';
	document.getElementById('bl'+j).value = '0';
	document.getElementById('reu'+j).value = '0';
	document.getElementById('td'+j).value = '0';
	document.getElementById('int'+j).value = '0';
	document.getElementById('sor'+j).value = '0';
	document.getElementById('jpv'+j).value = '0';
}

function check_limitation(j){
	var n=0;
	for(i=1;i<=16;i++){
		position_ok = document.getElementById('p'+i).options[document.getElementById('p'+i).selectedIndex].text;
		if(position_ok==position_name[j] ){
			n++;
		}
	}

	if(n>position_limit[j]) 
		return false;
	else 
		return true;
}

function set_default_value(o, j){
	var i = o.selectedIndex;
	document.getElementById('div_skills').style.display = 'none';

	if(check_limitation(i)){
		document.getElementById('ma'+j).value = position_ma[i];
		document.getElementById('st'+j).value = position_st[i];
		document.getElementById('ag'+j).value = position_ag[i];
		document.getElementById('av'+j).value = position_av[i];
		document.getElementById('c_lg'+j).innerHTML = translate(position_comp[i]);
		document.getElementById('c'+j).value = position_comp[i];
		document.getElementById('cout'+j).innerHTML = position_cout[i];
		document.getElementById('cout_v'+j).value = position_cout[i];
	}
	else {
		alert("Attention, vous n'avez droit qu'a " + position_limit[i] + " " + position_name[i]+ "(s)");
		o.selectedIndex = (o.options.length-1);
		reset_value(j);
	}

}

function generer_xml(){
	if(document.getElementById('dl_xml').value == 0) {
		var xml_win = window.open('','xml_win','resizable=yes,toolbar=no,status=no,scrollbars=yes,location=no,menubar=no,directories=no,width=640,height=480');
		document.getElementById('feuille').target='xml_win';
	}
	document.getElementById('feuille').action = 'export_xml.php';
	document.getElementById('feuille').submit();
	if(document.getElementById('dl_xml').value == 0) xml_win.focus();
	document.getElementById('feuille').target = '';
	document.getElementById('feuille').action = '/teamroster/';
}

function version_imprimable(){
	var print_win = window.open('','print_win','resizable=yes,toolbar=no,status=0,scrollbars=no,location=no,menubar=no,directories=no,width=810,height=460');
	document.getElementById('feuille').target = 'print_win';
	document.getElementById('feuille').action = 'printable_roster.php';
	document.getElementById('feuille').submit();
	print_win.focus();
	document.getElementById('feuille').target = '';
	document.getElementById('feuille').action = '/teamroster/';
}

function open_editeur_formation(){
	var edit_formation_win = window.open('','edit_formation_win','resizable=yes,toolbar=no,status=0,scrollbars=no,location=no,menubar=no,directories=no,width=700,height=460');
	document.getElementById('feuille').target = 'edit_formation_win';
	document.getElementById('feuille').action = 'edit_formation.php';
	document.getElementById('feuille').submit();
	edit_formation_win.focus();
	document.getElementById('feuille').target = '';
	document.getElementById('feuille').action = '/teamroster/';
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

var add_skill_to = 0;

function show_skill_popup(numero,race,position,liste_comp){

	var position_ok = document.getElementById('p'+numero).options[document.getElementById('p'+numero).selectedIndex].text;
	if(position_ok!="---"){
		remplir_liste_skill_avant(liste_comp);
		remplir_liste_skill_apres(position);
		add_skill_to = numero;

		if(navigator.userAgent.indexOf('MSIE')>0){
			document.getElementById('div_skills').style.left = event.x + document.body.scrollLeft;
			document.getElementById('div_skills').style.top = event.y + document.body.scrollTop;
		}
		else{
			document.getElementById('div_skills').style.left = 0;
			document.getElementById('div_skills').style.top = document.body.scrollTop;
		}
		document.getElementById('div_skills').style.display = 'block';
	}
	else {
		alert("Merci de choisir la position du joueur...");
		add_skill_to = 0;
		document.getElementById('div_skills').style.display = 'none';
	}
}

function close_skill_popup(){
	add_skill_to = 0;
	document.getElementById('div_skills').style.display = 'none';
}

function ajouter_skill() {
	destList = document.getElementById('liste_skill_avant');
	srcList = document.getElementById('liste_skill_apres');
	var len = destList.length;
	for(var i = 0; i < srcList.length; i++) {
		if ((srcList.options[i] != null) && (srcList.options[i].selected)) {
			var found = false;
			for(var count = 0; count < len; count++) {
				if (destList.options[count] != null) {
					if (srcList.options[i].text == destList.options[count].text) {
						found = true;
						break;
					}
				}
			}

			if (found != true) {
				destList.options[len] = new Option(srcList.options[i].text,srcList.options[i].value ); 
				srcList.options[i] = null;
				len++;
			}
		}
   }
}

function retirer_skill() {
	destList = document.getElementById('liste_skill_apres');
	srcList = document.getElementById('liste_skill_avant');
	var len = destList.length;
	for(var i = 0; i < srcList.length; i++) {
		if ((srcList.options[i] != null) && (srcList.options[i].selected)) {
			var found = false;
			for(var count = 0; count < len; count++) {
				if (destList.options[count] != null) {
					if (srcList.options[i].text == destList.options[count].text) {
						found = true;
						break;
					}
				}
			}

			if (found != true) {
				destList.options[len] = new Option(srcList.options[i].text,srcList.options[i].value); 
				srcList.options[i] = null;
				len++;
			}
			else srcList.options[i] = null;
		}
   }
}

function save_and_close_skill(){
	set_skills(document.getElementById('liste_skill_avant'),add_skill_to); 
	document.getElementById('div_skills').style.display = 'none';
}

function remplir_liste_skill_avant(skills){
	var destList = document.getElementById('liste_skill_avant');
	var len = destList.length;
	var i = 0;
	for (i=0; i < len; i++){
		destList.options[i] = null;
	}
	destList.length = 0;

	if(skills!='' && skills!=undefined){
		var separator = ',';
		var stringArray = skills.split(separator);
		for (i=0; i < stringArray.length; i++){
			len = destList.length;
			destList.options[len] = new Option(skills_en_to_fr[stringArray[i]],stringArray[i]);	
		}
	}
}

function remplir_liste_skill_apres(position){
	var destList = document.getElementById('liste_skill_apres');
	var len = destList.length;
	var i = 0;
	for (i=0; i < len; i++){
		destList.options[i] = null;
	}
	destList.length = 0;

	document.getElementById('skill_cat_possible').innerHTML = skills[position];

	var cat = skills[position].split(",");
	for (var j=0; j<cat.length; j++){
		var c = cat[j];
		if(skill_list[c]) {
			for(var h=0;h<skill_list[c].length;h++){
				len = destList.length;
				destList.options[len] = new Option(skills_en_to_fr[skill_list[c][h]],skill_list[c][h]);	
			}
		}

		if(trait_list[c]){
			for(var h=0;h<trait_list[c].length;h++){
				len = destList.length;
				destList.options[len] = new Option(skills_en_to_fr[trait_list[c][h]],trait_list[c][h]);	
			}	
		}
	}		
}

function set_action_label(s){
	if( !xmlfile_picker_visible )
		document.getElementById('action_label').innerHTML = s;
}

var xmlfile_picker_visible = false;

function show_xmlfile_picker(){
	xmlfile_picker_visible = false;
	set_action_label(document.getElementById('xmlfile_div').innerHTML);
	xmlfile_picker_visible = true;
}

function close_xmlfile_picker(){
	xmlfile_picker_visible = false;
	set_action_label('');
}

function importer_xml_team(){
	document.getElementById('feuille').target = '_self';
	document.getElementById('feuille').action = '/teamroster/';
	document.getElementById('feuille').todo.value = 'loader_xml';
	document.getElementById('feuille').submit();
}

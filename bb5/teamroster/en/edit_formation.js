
function MM() {
	if(document.img_souris.src.indexOf('../pions/vide.gif')==-1) {
		div_souris.style.left = event.clientX-22;
		div_souris.style.top = event.clientY-22;
	}
	return false;
}
document.onmousemove = MM;

var current_formation = 1;
var selected_player = new Array();
selected_player[1] = 0;
selected_player[2] = 0;
selected_player[3] = 0;
selected_player[4] = 0;
var player_placed = new Array();
player_placed[1] = new Array();
player_placed[2] = new Array();
player_placed[3] = new Array();
player_placed[4] = new Array();
var terrain = new Array();
terrain[1] = new Array(); // ligne
terrain[2] = new Array(); // ligne
terrain[3] = new Array(); // ligne
terrain[4] = new Array(); // ligne
for(t=0;t<15;t++) terrain[1][t] = new Array(); //colonne
for(t=0;t<15;t++) terrain[2][t] = new Array(); //colonne
for(t=0;t<15;t++) terrain[3][t] = new Array(); //colonne
for(t=0;t<15;t++) terrain[4][t] = new Array(); //colonne

terrain[1][1][12] = 1;
terrain[1][2][12] = 2;
terrain[1][4][12] = 3;
terrain[1][5][12] = 4;
terrain[1][6][12] = 5;
terrain[1][8][12] = 6;
terrain[1][9][12] = 7;
terrain[1][10][12] = 8;
terrain[1][12][12] = 9;
terrain[1][13][12] = 10;
terrain[1][7][9] = 11;

terrain[2][1][12] = 1;
terrain[2][3][12] = 2;
terrain[2][4][12] = 3;
terrain[2][5][12] = 4;
terrain[2][7][12] = 5;
terrain[2][9][12] = 6;
terrain[2][10][12] = 7;
terrain[2][11][12] = 8;
terrain[2][13][12] = 9;
terrain[2][5][9] = 10;
terrain[2][9][9] = 11;

terrain[3][1][11] = 1;
terrain[3][3][12] = 2;
terrain[3][4][11] = 3;
terrain[3][5][12] = 4;
terrain[3][7][12] = 5;
terrain[3][9][12] = 6;
terrain[3][10][11] = 7;
terrain[3][11][12] = 8;
terrain[3][13][11] = 9;
terrain[3][5][8] = 10;
terrain[3][9][8] = 11;

terrain[4][1][11] = 1;
terrain[4][3][12] = 2;
terrain[4][6][12] = 3;
terrain[4][5][12] = 4;
terrain[4][7][9] = 5;
terrain[4][9][12] = 6;
terrain[4][8][12] = 7;
terrain[4][11][12] = 8;
terrain[4][13][11] = 9;
terrain[4][4][7] = 10;
terrain[4][10][7] = 11;

function change_bg(o,x,y){
	if( selected_player[current_formation]<=0 ) {
		if( terrain[current_formation][x][y]>0 ) {
			document.all['td_case'+x+'_'+y].style.backgroundColor='';
			document.all['td_case'+x+'_'+y].innerText='';
			set_player_placed(terrain[current_formation][x][y],0);
			toggle_selected(terrain[current_formation][x][y]);	
			terrain[current_formation][x][y] = 0;
		}
		return;
	}	

	if(terrain[current_formation][x][y]>0){
		avant = terrain[current_formation][x][y];

		set_player_placed(avant,0);
		terrain[current_formation][x][y] = 0;
		
		document.all['td_case'+x+'_'+y].style.backgroundColor='#FFFFFF';
		document.all['td_case'+x+'_'+y].innerHTML = "<font class='titre'><b>"+selected_player[current_formation]+"</b></font>";

		set_player_placed(selected_player[current_formation],1);
		terrain[current_formation][x][y] = selected_player[current_formation];
		toggle_selected(selected_player[current_formation]);

		toggle_selected(avant);
	}
	else {
		document.all['td_case'+x+'_'+y].style.backgroundColor='#FFFFFF';
		document.all['td_case'+x+'_'+y].innerHTML = "<font class='titre'><b>"+selected_player[current_formation]+"</b></font>";

		set_player_placed(selected_player[current_formation],1);
		terrain[current_formation][x][y] = selected_player[current_formation];
		toggle_selected(selected_player[current_formation]);
	}
}

function set_player_placed(p,b){
	player_placed[current_formation][p] = b;
}

function update_bg_color(){
	for (i=0; i < document.all.table_membre.rows.length; i++) {
		if(player_placed[current_formation][document.all.table_membre.rows(i).name]==1) {
			document.all.table_membre.rows(i).style.backgroundColor='#F7F2D4';
		}
		else if( document.all.table_membre.rows(i).name == selected_player[current_formation]){
			document.all.table_membre.rows(i).style.backgroundColor='#FFDDDD';
		} 
		else {
			document.all.table_membre.rows(i).style.backgroundColor='#FFFFEE';
		}
    }
}

function toggle_selected(j,ligne){
	if(selected_player[current_formation]!=j && player_placed[current_formation][j]==1) { return; } 

	if(selected_player[current_formation]==j) {
		selected_player[current_formation]=0;
		document.img_souris.src = "../pions/vide.gif";
	}
	else {
		selected_player[current_formation]=j;
		document.img_souris.src = "../pions/"+selected_player[current_formation]+"p.gif";
		div_souris.style.left = event.clientX - 22;
		div_souris.style.top = event.clientY - 22;
	}
	update_bg_color();
	
	if(ligne && selected_player[current_formation]==j) {
		ligne.style.backgroundColor='#FFDDDD';
	}
	
}

var myimagesp=new Array();

function preloadimages(){

	for (i=1;i<=16;i++){
		window.status = "Loading pions/" + i +"p.gif ...";
		myimagesp[i]=new Image();
		myimagesp[i].src="../pions/" + i + "p.gif";
	}

	window.status = "";
}

function generer_xml(){
	var params = "";
	for(h=0;h<13;h++){
		for(l=0;l<15;l++){
			if(terrain[1][l][h]>0) {
				params += "&a"+terrain[1][l][h]+"="+l+","+h+"";
			}
			if(terrain[2][l][h]>0) {
				params += "&b"+terrain[2][l][h]+"="+l+","+h+"";
			}
			if(terrain[3][l][h]>0) {
				params += "&c"+terrain[3][l][h]+"="+l+","+h+"";		
			}
			if(terrain[4][l][h]>0) {
				params += "&d"+terrain[4][l][h]+"="+l+","+h+"";
			}
		}
	}

	params += "&dl_xml="+document.editeur_formation.dl_xml.value;
	if(document.editeur_formation.dl_xml.value==0) {
		var xml_win = window.open('export_xml_formation.php?'+params,'xml_win','resizable=yes,toolbar=no,status=no,scrollbars=yes,location=no,menubar=no,directories=no,width=640,height=480');
	}
	else {
		document.location = 'export_xml_formation.php?'+params;
	}
}

function update_terrain(){
	for(h=0;h<13;h++){
		for(l=0;l<15;l++){
			if(terrain[current_formation][l][h]>0) {
				document.all['td_case'+l+'_'+h].style.backgroundColor='#FFFFFF';
				document.all['td_case'+l+'_'+h].innerHTML = "<font class='titre'><b>"+terrain[current_formation][l][h]+"</b></font>";
				player_placed[current_formation][terrain[current_formation][l][h]] = 1; 
			}
			else {
				document.all['td_case'+l+'_'+h].style.backgroundColor='';
				document.all['td_case'+l+'_'+h].innerHTML = '';
			}
		}
	}
}

function change_formation(i){
	current_formation = i+1;
	window.status = "Current formation = " + current_formation;
	update_terrain();
	update_bg_color();
	document.img_souris.src = "../pions/vide.gif";
}

function check_formation(){
	var nb_left = 0;
	var nb_right = 0;
	var nb_center = 0;
	var nb_total = 0;

	for(h=0;h<13;h++){
		for(l=0;l<15;l++){
			if(terrain[current_formation][l][h]>0) {
				nb_total++;
				if(l<4) nb_left++;
				else if(l>=11) nb_right++;
				else {
					if(h==12) nb_center++;
				}
			}
		}
	}

	var msg = "";
	if(nb_left>2) msg += nb_left + " players in left corridor (max=2)\n";
	if(nb_right>2) msg +=  nb_right + " players in right corridor (max=2)\n";
	if(nb_center<3) msg += "Only "+nb_center+" players are on the center line (min=3)\n";
	if(nb_total>11) msg += nb_total+" players on the field (max=11)\n";

	if(msg!="") alert(msg);
	else alert("This formation is OK !");
}
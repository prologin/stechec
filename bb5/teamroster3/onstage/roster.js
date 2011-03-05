function legalize(player) {
	setRoster(player,"POSITION[]",1)
	legalize_skills = getFromRoster(player,"SKILLS[]")
	legalize_skills = legalize_skills.replace(/,Loner,/,",")
	legalize_skills = legalize_skills.replace(/,Loner/,"")
	legalize_skills = legalize_skills.replace(/Loner,/,"")
	legalize_skills = legalize_skills.replace(/Loner/,"")
	setRoster(player,"SKILLS[]",legalize_skills)
	hide('jm_box')
}

function checkName(row) {
	if( isPlayerAssigned(row) == false ) {
		alert(warning[2])
		setRoster(row,"NAME[]","")
	}
}		

function fillPlayerStats(row) {
	position = getFromRoster(row,"POSITION[]")
	
	if ( pos = isQtyProblem() ) {
		alert(warning[0] + " " + stats[pos][6] + " '" + stats[pos][0] + "'.")
		position = 0
	}
	
	if ( position == positions && !(isJourneymanAllowed()) ) {
		healthy_players = getFromRoster(0, "HEALTHY") - 1
		alert(warning[1] + " " + healthy_players + ".")
		position = 0
	}
	setRoster(row,"NAME[]","")
	setRoster(row,"MA[]",stats[position][1])
	setRoster(row,"ST[]",stats[position][2])
	setRoster(row,"AG[]",stats[position][3])
	setRoster(row,"AV[]",stats[position][4])
	document.getElementsByName("VALUE[]")[row].className = 'healthy'
	setRoster(row,"VALUE[]",stats[position][5])
	setRoster(row,"POSITION[]",position)
	setRoster(row,"SKILLS[]",skills[position].join(","))
	setRoster(row,"INJURIES[]","")
	setRoster(row,"COMP[]","")
	setRoster(row,"TD[]","")
	setRoster(row,"INT[]","")
	setRoster(row,"CAS[]","")
	setRoster(row,"MVP[]","")
	setRoster(row,"SPP[]","")
	calcTeamValue()
}

function setInjury(string) {
	output = document.getElementById('ROSTER').TEMP2.value
	row = output - 1
	
	if(string == 'M')
	{
		injuries = getFromRoster(row,"INJURIES[]")
		setRoster(row,"INJURIES[]",'M')
		if (injuries != '') { concatRoster(row,"INJURIES[]",injuries) }
	}
	
	if(string == 'N')
	{
		if(!isPlayerInjured(row))
		{
			injuries = getFromRoster(row,"INJURIES[]")
			setRoster(row,"INJURIES[]",'M')
			if (injuries != '') { concatRoster(row,"INJURIES[]",injuries) }
		}
		concatRoster(row,"INJURIES[]",'N')
	}
	
	if(string == "")
	{
		if ( document.getElementsByName("INJURIES[]")[row].value.charAt(1) == "," ) {
			setRoster(row,"INJURIES[]", document.getElementsByName("INJURIES[]")[row].value.slice(2))
		}
		else {
			setRoster(row,"INJURIES[]",'')
		}
	}
	
	findInjuredPlayers()
	countHealthyPlayers()	
	calcTeamValue()
	hide('inj_box')
}

function changeStat(stat,operator) {
	if (operator == "p") {
		row = document.getElementById('ROSTER').TEMP1.value - 1	
	}
	if (operator == "m") {
		row = document.getElementById('ROSTER').TEMP2.value - 1	
	}
	if(isPlayerAssigned(row) == true) {
		position= getFromRoster(row,"POSITION[]")
		ma_diff = getFromRoster(row,"MA[]") - stats[position][1]
		st_diff = getFromRoster(row,"ST[]") - stats[position][2]
		ag_diff = getFromRoster(row,"AG[]") - stats[position][3]
		av_diff = getFromRoster(row,"AV[]") - stats[position][4]
		if (operator == "p") {
			if (document.getElementsByName(stat+"[]")[row].value < 10) {
				statlesserten = new Boolean(true)
			} else { 
				statlesserten = new Boolean(false) 
			}
			switch(stat) {
				case 'MA':
					if (ma_diff < 2 && statlesserten == true) {
						document.getElementsByName("MA[]")[row].value++
						changeRoster(row,"VALUE[]",30000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+MA)
						}
					} else { alert(warning[4]) }
					break
				case 'ST':
					if (st_diff < 2 && statlesserten == true) {
						document.getElementsByName("ST[]")[row].value++
						changeRoster(row,"VALUE[]",50000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+ST)
						}
					} else { alert(warning[4]) }
					break
				case 'AG':
					if (ag_diff < 2 && statlesserten == true) {
						document.getElementsByName("AG[]")[row].value++
						changeRoster(row,"VALUE[]",40000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+AG)
						}						
					} else { alert(warning[4]) }
					break
				case 'AV':
					if (av_diff < 2 && statlesserten == true) {
						document.getElementsByName("AV[]")[row].value++
						changeRoster(row,"VALUE[]",30000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+AV)
						}						
					} else { alert(warning[4]) }
					break
				default:
					alert("No valid stat parameter given.")
			}
		}	
		if (operator == "m") {
			if (document.getElementsByName(stat+"[]")[row].value > 1) {
				statgreaterone = new Boolean(true)
			} else { 
				statgreaterone = new Boolean(false) 
			}
			switch(stat) {
				case 'MA':
					if (ma_diff > -2 && statgreaterone == true) {
						document.getElementsByName("MA[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJURIES[]","-"+MA)
						}
					} else { alert(warning[5]) }
					break
				case 'ST':
					if (st_diff > -2 && statgreaterone == true) {
						document.getElementsByName("ST[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJURIES[]","-"+ST)
						}						
					} else { alert(warning[5]) }
					break
				case 'AG':
					if (ag_diff > -2 && statgreaterone == true) {
						document.getElementsByName("AG[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJURIES[]","-"+AG)							
						}						
					} else { alert(warning[5]) }
					break
				case 'AV':
					if (av_diff > -2 && statgreaterone == true) {
						document.getElementsByName("AV[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJURIES[]","-"+AV)
						}
					} else { alert(warning[5]) }
					break
				default:
					alert("No valid stat parameter given.")
			}
		}
	} else { alert(warning[2]) }
	findInjuredPlayers()
	calcTeamValue()
	hide('skill_box')
	hide('inj_box')
}

function calcPlayerSPP(row) {
	if(isPlayerAssigned(row) != false) {
		COMP = getFromRoster(row,"COMP[]")
		if (isNaN(COMP) || COMP<0) { 
			COMP = prompt(warning[8])
			if (isNaN(COMP) || COMP<0) { COMP = 0 }
		}
		TD = getFromRoster(row,"TD[]")
		if (isNaN(TD) || TD<0) { 
			TD = prompt(warning[8])
			if (isNaN(TD) || TD<0) { TD = 0 }
		}
		INT = getFromRoster(row,"INT[]")
		if (isNaN(INT) || INT<0) { 
			INT = prompt(warning[8])
			if (isNaN(INT) || INT<0) { INT = 0 }
		}
		CAS = getFromRoster(row,"CAS[]")
		if (isNaN(CAS) || CAS<0) { 
			CAS = prompt(warning[8])
			if (isNaN(CAS) || CAS<0) { CAS = 0 }
		}
		MVP = getFromRoster(row,"MVP[]")
		if (isNaN(MVP) || MVP<0) { 
			MVP = prompt(warning[8])
			if (isNaN(MVP) || MVP<0) { MVP = 0 }
		}
		setRoster(row,"COMP[]",COMP)
		setRoster(row,"TD[]",TD)
		setRoster(row,"INT[]",INT)
		setRoster(row,"CAS[]",CAS)
		setRoster(row,"MVP[]",MVP)
		setRoster(row,"SPP[]",COMP*1 + TD*3 + INT*2 + CAS*2 + MVP*5)
	} else { 
		alert(warning[2])
		setRoster(row,"COMP[]","")
		setRoster(row,"TD[]","")
		setRoster(row,"INT[]","")
		setRoster(row,"CAS[]","")
		setRoster(row,"MVP[]","")	
	}
}

function calcExtraValue(j) {
	if(j == 16) {
		if(document.getElementById('ROSTER').REROLLS.value>8) {
			document.getElementById('ROSTER').REROLLS.value=8
			alert(warning[10])
		}
		setRoster(j, "VALUE[]", document.getElementById('ROSTER').REROLLS.value*reroll_cost)
	}
	if(j == 17) {
		setRoster(j, "VALUE[]", document.getElementById('ROSTER').FANFACTOR.value*10000)
	}
	if(j == 18) {
		setRoster(j, "VALUE[]", document.getElementById('ROSTER').COACHES.value*10000)
	}
	if(j == 19) {
		setRoster(j, "VALUE[]", document.getElementById('ROSTER').CHEERLEADERS.value*10000)
	}
	if(j == 20) {
		if (apothecary == false) {
			alert(warning[11])
			document.getElementById('ROSTER').APOTHECARY.value = 0
		}
		if (document.getElementById('ROSTER').APOTHECARY.value > 1) {
			alert(warning[12])
			document.getElementById('ROSTER').APOTHECARY.value = 1
		}
	setRoster(j, "VALUE[]", document.getElementById('ROSTER').APOTHECARY.value*50000)
	}
	calcTeamValue()
}

function calcTeamValue() {
	teamvalue = 0
	for ( k=0; k<=15; k++) { // players
		value = parseInt(getFromRoster(k, "VALUE[]"))
		if(isNaN(value) == false && isPlayerInjured(k) == false ) {
			teamvalue = teamvalue + value
    	}
    }
	for ( k=16; k<=20; k++) { // extras
		value = parseInt(getFromRoster(k, "VALUE[]"))
		if(isNaN(value) == false ) {
			teamvalue = teamvalue + value
    	}
    }
	document.getElementById('ROSTER').TEAMVALUE.value = teamvalue
	
	// color teamvalue red if it is > 1.000.000 gp
	if (teamvalue > 1000000) {
		document.getElementById('ROSTER').TEAMVALUE.style.color = "red"
	}
	else {
		document.getElementById('ROSTER').TEAMVALUE.style.color = "black"
	}
}

function showInjBox(row) {
	if(isPlayerAssigned(row) == true)
	{
		if( show('inj_box') == true )
		{
			output = row + 1
			document.getElementById('ROSTER').TEMP2.value = output
			
			if(isPlayerInjured(row) == true)
			{
				document.getElementById('miss').style.display = 'none';
				document.getElementById('ok').style.display = 'block';
			}
			else
			{
				document.getElementById('miss').style.display= 'block';
				document.getElementById('ok').style.display = 'none';
			}
		}	
	}
	else
	{
		alert(warning[2]) 
	}
}

function showJmBox() {
	if ( show('jm_box') == true ) {
		for ( i = 0; i < 16; i++ ) {
			if ( isPlayerAssigned(i) != 2 ) {
				document.getElementById('jm'+i).style.display = "none";
			}
			else {
				document.getElementById('jm'+i).style.display = "block";
			}
		}
	}
}

function showSkillBox(row) {
	if(isPlayerAssigned(row) == true) {
		if ( show('skill_box') == true ) {
			position = getFromRoster(row,"POSITION[]")
			output = row + 1
			document.getElementById('ROSTER').TEMP3.value = 0
			document.getElementById('ROSTER').SKILLDEST.options.length = 0
			document.getElementById('ROSTER').TEMP1.value = output
			choice_normal = stats[position][8].split("")
			choice_double = stats[position][9].split("")
			chosen_skills = new Array()
			if (document.getElementsByName("SKILLS[]")[row].value != "") {
				chosen_skills = document.getElementsByName("SKILLS[]")[row].value.split(",")
			} else {
				chosen_skills.length = 0
			}
			possible_skills_n = new Array()
			possible_skills_d = new Array()

			for (i=0; i < choice_normal.length; i++) {
				if (choice_normal[i] == "g") { possible_skills_n = possible_skills_n.concat(skills_g) }
				if (choice_normal[i] == "a") { possible_skills_n = possible_skills_n.concat(skills_a) }
				if (choice_normal[i] == "p") { possible_skills_n = possible_skills_n.concat(skills_p) }
				if (choice_normal[i] == "s") { possible_skills_n = possible_skills_n.concat(skills_s) }
				if (choice_normal[i] == "m") { possible_skills_n = possible_skills_n.concat(skills_m) }
				if (choice_normal[i] == "e") { possible_skills_n = possible_skills_n.concat(skills_e) }
			}
			for (i=0; i < choice_double.length; i++) {
				if (choice_double[i] == "g") { possible_skills_d = possible_skills_d.concat(skills_g) }
				if (choice_double[i] == "a") { possible_skills_d = possible_skills_d.concat(skills_a) }
				if (choice_double[i] == "p") { possible_skills_d = possible_skills_d.concat(skills_p) }
				if (choice_double[i] == "s") { possible_skills_d = possible_skills_d.concat(skills_s) }
				if (choice_double[i] == "m") { possible_skills_d = possible_skills_d.concat(skills_m) }
				if (choice_double[i] == "e") { possible_skills_d = possible_skills_d.concat(skills_e) }
			}
			
			impossible_choice = new Array("g","a","p","s","m","e")
			eliminateDoubles(impossible_choice,choice_normal)
			eliminateDoubles(impossible_choice,choice_double)
			impossible_skills = new Array()			
			for (i=0; i < impossible_choice.length; i++) {
				if (impossible_choice[i] == "g") { impossible_skills = impossible_skills.concat(skills_g) }
				if (impossible_choice[i] == "a") { impossible_skills = impossible_skills.concat(skills_a) }
				if (impossible_choice[i] == "p") { impossible_skills = impossible_skills.concat(skills_p) }
				if (impossible_choice[i] == "s") { impossible_skills = impossible_skills.concat(skills_s) }
				if (impossible_choice[i] == "m") { impossible_skills = impossible_skills.concat(skills_m) }
				if (impossible_choice[i] == "e") { impossible_skills = impossible_skills.concat(skills_e) }
			}
			
			eliminateDoubles(impossible_skills,chosen_skills)
			eliminateDoubles(possible_skills_n,chosen_skills)
			eliminateDoubles(possible_skills_d,chosen_skills)
			
			if ( isStringInArray(skills_s[1],chosen_skills) ) { 
				deleteArrayElement(skills_g[4],possible_skills_n)
				deleteArrayElement(skills_g[4],possible_skills_d)
				impossible_skills.push(skills_g[4])
			}

			if ( isStringInArray(skills_g[4],chosen_skills) ) { 
				deleteArrayElement(skills_s[1],possible_skills_n)
				deleteArrayElement(skills_s[1],possible_skills_d)
				impossible_skills.push(skills_s[1])
			}
				
			arrayToOptions(impossible_skills,'SKILLSRCF',0)
			arrayToOptions(possible_skills_n,'SKILLSRCN',20000)
			arrayToOptions(possible_skills_d,'SKILLSRCD',30000)
			arrayToOptions(chosen_skills,'SKILLDEST',0)
		}
	}
	else { 
		alert(warning[2]) 
	}
}

function showBox(ID) {
	// have to use the return value of show()
	shown = show(ID)
}

function addskill(repertory) {
	if (repertory == "d") { 
		var x = document.getElementById('ROSTER').SKILLSRCD
	} 
	else if (repertory == "n") {
		var x = document.getElementById('ROSTER').SKILLSRCN
	} 
	else {
		var x = document.getElementById('ROSTER').SKILLSRCF
	}
	
	selected_name = x.options[x.selectedIndex].text
	selected_value = x.options[x.selectedIndex].value
	destination_index = document.getElementById('ROSTER').SKILLDEST.options.length
	impossible_index = document.getElementById('ROSTER').SKILLSRCF.options.length
	
	if (selected_name == skills_g[4]) {
		if ( deleteSelectElement(skills_s[1],'SKILLSRCD') == true ) {
			document.getElementsByName('SKILLSRCF')[0].options[impossible_index] = new Option(skills_s[1],0)
		}
		if ( deleteSelectElement(skills_s[1],'SKILLSRCN') == true ) {
			document.getElementsByName('SKILLSRCF')[0].options[impossible_index] = new Option(skills_s[1],0)
		}
	}
	
	if (selected_name == skills_s[1]) {
		if ( deleteSelectElement(skills_g[4],'SKILLSRCD') == true ) {
			document.getElementsByName('SKILLSRCF')[0].options[impossible_index] = new Option(skills_g[4],0)
		}
		if ( deleteSelectElement(skills_g[4],'SKILLSRCN') == true ) {
			document.getElementsByName('SKILLSRCF')[0].options[impossible_index] = new Option(skills_g[4],0)
		}
	}
	
	document.getElementById('ROSTER').SKILLDEST.options[destination_index] = new Option(selected_name, selected_value)
	
	value_old = parseInt(document.getElementById('ROSTER').TEMP3.value)
	value_add = parseInt(x.options[x.selectedIndex].value)
	document.getElementById('ROSTER').TEMP3.value = value_old + value_add
	
	x.remove(x.selectedIndex)
	setSkillchanges()
}

function remskill() {
	row = parseInt(document.getElementById('ROSTER').TEMP1.value - 1)
	position = getFromRoster(row,"POSITION[]")
	
	var x = document.getElementById('ROSTER').SKILLDEST
	selectedskill = x.options[x.selectedIndex].text
	
	if (isStringInArray(selectedskill,skills[position]) == true) {
		alert(warning[6])
	}
	else if (selectedskill == "+MA" || selectedskill == "+ST" || selectedskill == "+AG" || selectedskill == "+AV") {
		alert(warning[7])
	} 
	else {
		repertory = findSkill(selectedskill,position)
		if (repertory == "normal") {
			document.getElementById('ROSTER').TEMP3.value = document.getElementById('ROSTER').TEMP3.value - 20000
			
			y = document.getElementById('ROSTER').SKILLSRCN.options
			index = y.length
			y[index] = new Option(selectedskill, 20000)
		}
		else if ( repertory == "double") {
			document.getElementById('ROSTER').TEMP3.value = document.getElementById('ROSTER').TEMP3.value - 30000
			
			y = document.getElementById('ROSTER').SKILLSRCD.options
			index = y.length
			y[index] = new Option(selectedskill, 20000)			
		}
		else if ( repertory == "forbidden") {
			y = document.getElementById('ROSTER').SKILLSRCF.options
			index = y.length
			y[index] = new Option(selectedskill, 0)
		}		
		
		if ( selectedskill == skills_s[1]) {

				if ( deleteSelectElement(skills_g[4],'SKILLSRCF') == true ) {

					if ( isStringInArray(skills_g[4],possible_skills_n) ) {
						index = document.getElementById('ROSTER').SKILLSRCN.options.length
						document.getElementById('ROSTER').SKILLSRCN.options[index] = new Option(skills_g[4], 30000)
					}
					else if ( isStringInArray(skills_g[4],possible_skills_n) ) {
						index = document.getElementById('ROSTER').SKILLSRCN.options.length
						document.getElementById('ROSTER').SKILLSRCN.options[index] = new Option(skills_g[4], 20000)
					}
				}
		}
				
		if ( selectedskill == skills_g[4]) {

				if ( deleteSelectElement(skills_s[1],'SKILLSRCF') == true ) {

					if ( isStringInArray(skills_s[1],possible_skills_n) ) {
						index = document.getElementById('ROSTER').SKILLSRCN.options.length
						document.getElementById('ROSTER').SKILLSRCN.options[index] = new Option(skills_s[1], 30000)
					}
					else if ( isStringInArray(skills_s[1],possible_skills_n) ) {
						index = document.getElementById('ROSTER').SKILLSRCN.options.length
						document.getElementById('ROSTER').SKILLSRCN.options[index] = new Option(skills_s[1], 20000)
					}
				}
		}
		
		x.remove(x.selectedIndex)
		setSkillchanges()
	}
}

function setSkillchanges() {
	row = parseInt(document.getElementById('ROSTER').TEMP1.value - 1)
	skillString = ""
	for (i=0; i < document.getElementById('ROSTER').SKILLDEST.options.length; i++) {
		skillString = skillString + document.getElementById('ROSTER').SKILLDEST.options[i].text
			if(i < (document.getElementById('ROSTER').SKILLDEST.options.length - 1)) {
				skillString = skillString + ","
			}
	}
	value_old = parseInt(getFromRoster(row, "VALUE[]"))
	value_change = parseInt(document.getElementById('ROSTER').TEMP3.value)
	setRoster(row, "SKILLS[]", skillString)
	setRoster(row, "VALUE[]", value_old + value_change)
	calcTeamValue()
	hide('skill_box')
}

function save() {
	if ( getFromRoster(0, "TEAM") == "" ) {
		alert(warning[13])
	} 
	else {
		document.getElementById('ROSTER').submit()
	}
}

function show(ID) {
	if (box_visible == true) {
		alert(warning[9]);
		return false
	}
	else {
		box_visible = true;
		document.getElementById(ID).className = 'element_visible';
		return true
	}
}

function hide(ID) {
	document.getElementById(ID).className = 'element_hidden';
	box_visible = false;
}

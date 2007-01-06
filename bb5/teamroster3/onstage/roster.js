function setSelects() {
		for ( i=0; i < 16; i++) {
			document.getElementsByName("POSITION[]")[i].selectedIndex = arrpositions[i]
		}
}

function countHealthyPlayers() {

	// first it counts how many healthy players the roster has
	
	healthy_players = 0
	for ( i=0; i<16; i++ ) {
		if ( isPlayerAssigned(i) != false && isPlayerInjured(i) == false ) {
			healthy_players++
		}
	}
	document.getElementsByName("HEALTHY")[0].value = healthy_players
	
	// then it will return a boolean to say if journeymen are allowed
	
	return healthy_players
}

function legalize(player) {
	setRoster(player,"POSITION[]",0)
	legalize_skills = getFromRoster(player,"SKILLS[]")
	legalize_skills = legalize_skills.replace(/,Loner,/,",")
	legalize_skills = legalize_skills.replace(/,Loner/,"")
	legalize_skills = legalize_skills.replace(/Loner,/,"")
	legalize_skills = legalize_skills.replace(/Loner/,"")
	setRoster(player,"SKILLS[]",legalize_skills)
}

function isJourneymanAllowed() {
	if ( countHealthyPlayers() < 12 ) {
		return true
	}
	else {
		return false
	}
}

function isPlayerInjured(row) {
	if ( document.getElementsByName("INJ[]")[row].value.charAt(0) == "M" ) {
		return true
	}
	else {
		return false
	}
}			

function findInjuredPlayers() {
	for ( i=0; i<16; i++) {
		if ( isPlayerInjured(i) == true ) {
			document.getElementsByName("VALUE[]")[i].className = 'injured'
		}
		else {
			document.getElementsByName("VALUE[]")[i].className = 'healthy'
		}
	}
}

function setlogo() {
	document.getElementById("BADGE").src = "data/logos/"+logo
}

function checkName(row) {
	if(!(isPlayerAssigned(row) != false)) {
		alert(warning[2])
		setRoster(row,"NAME[]","")
	}
}		

function fillPlayerStats(row) {
	position = getFromRoster(row,"POSITION[]")
	
	if ( isQtyProblem() ) {
		position = positions
	}
	
	if ( position == (positions - 1) && !(isJourneymanAllowed()) ) {
		healthy_players = document.getElementsByName("HEALTHY")[0].value
		alert(healthy_players + " " + warning[1])
		position = positions
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
	setRoster(row,"INJ[]","")
	setRoster(row,"COMP[]","")
	setRoster(row,"TD[]","")
	setRoster(row,"INT[]","")
	setRoster(row,"CAS[]","")
	setRoster(row,"MVP[]","")
	setRoster(row,"SPP[]","")
	calcTeamValue()
}

function setInjury() {
	row = parseInt(document.getElementById('ROSTER').TEMP2.value-1)
	if (document.getElementById('ROSTER').M.checked == false) {
		setRoster(row,"INJ[]","")
		document.getElementsByName("VALUE[]")[row].className = 'healthy'
	}
	if (document.getElementById('ROSTER').M.checked == true) {
		setRoster(row,"INJ[]","M")
		document.getElementsByName("VALUE[]")[row].className = 'injured'
	}
	for (i=0; i<document.getElementById('ROSTER').N.value; i++) { 
		concatRoster(row,"INJ[]","N")
	}
	for (i=0; i<document.getElementById('ROSTER').TEMPMA.value; i++) {
		concatRoster(row,"INJ[]","-MA")
	}
	for (i=0; i<document.getElementById('ROSTER').TEMPST.value; i++) {
		concatRoster(row,"INJ[]","-ST")
	}
	for (i=0; i<document.getElementById('ROSTER').TEMPAG.value; i++) {
		concatRoster(row,"INJ[]","-AG")
	}
	calcTeamValue()
	countHealthyPlayers()
	hideLayer('inj_box')
}

function setNiggling(operator) {
	nigglings = parseInt(document.getElementById('ROSTER').N.value)
	if (operator == "plus") {
		nigglings++
		document.getElementById('ROSTER').M.checked = true		
	}
	if (operator == "minus" && nigglings > 0) {
		nigglings--
	}
	document.getElementById('ROSTER').N.value = nigglings
}

function changeStat(stat,operator,row) {
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
					} else { alert(warning[5]) }
					break
				case 'ST':
					if (st_diff < 2 && statlesserten == true) {
						document.getElementsByName("ST[]")[row].value++
						changeRoster(row,"VALUE[]",50000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+ST)
						}
					} else { alert(warning[5]) }
					break
				case 'AG':
					if (ag_diff < 2 && statlesserten == true) {
						document.getElementsByName("AG[]")[row].value++
						changeRoster(row,"VALUE[]",40000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+AG)
						}						
					} else { alert(warning[5]) }
					break
				case 'AV':
					if (av_diff < 2 && statlesserten == true) {
						document.getElementsByName("AV[]")[row].value++
						changeRoster(row,"VALUE[]",30000)
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"SKILLS[]","+"+AV)
						}						
					} else { alert(warning[5]) }
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
							concatRoster(row,"INJ[]","-"+MA)
						}
					} else { alert(warning[6]) }
					break
				case 'ST':
					if (st_diff > -2 && statgreaterone == true) {
						document.getElementsByName("ST[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJ[]","-"+ST)
						}						
					} else { alert(warning[6]) }
					break
				case 'AG':
					if (ag_diff > -2 && statgreaterone == true) {
						document.getElementsByName("AG[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJ[]","-"+AG)							
						}						
					} else { alert(warning[6]) }
					break
				case 'AV':
					if (av_diff > -2 && statgreaterone == true) {
						document.getElementsByName("AV[]")[row].value--
						if(document.getElementById('ROSTER').VERBOSE.checked == true) {
							concatRoster(row,"INJ[]","-"+AV)
						}
					} else { alert(warning[6]) }
					break
				default:
					alert("No valid stat parameter given.")
			}
		}
	} else { alert(warning[7]) }
	calcTeamValue()
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
		document.getElementsByName("SPP[]")[row].value = COMP*1 + TD*3 + INT*2 + CAS*2 + MVP*5
	} else { 
		alert(warning[4])
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
			alert(warning[9])
		}
		document.getElementsByName("VALUE[]")[j].value = document.getElementById('ROSTER').REROLLS.value*reroll_cost
	}
	if(j == 17) {
		document.getElementsByName("VALUE[]")[j].value = document.getElementById('ROSTER').FANFACTOR.value*10000
	}
	if(j == 18) {
		document.getElementsByName("VALUE[]")[j].value = document.getElementById('ROSTER').COACHES.value*10000
	}
	if(j == 19) {
		document.getElementsByName("VALUE[]")[j].value = document.getElementById('ROSTER').CHEERLEADERS.value*10000
	}
	if(j == 20) {
		if (apothecary == false) {
			alert(warning[10])
			document.getElementById('ROSTER').APOTHECARY.value = 0
		}
		if (document.getElementById('ROSTER').APOTHECARY.value > 1) {
			alert(warning[15])
			document.getElementById('ROSTER').APOTHECARY.value = 1
		}
	document.getElementsByName("VALUE[]")[j].value = document.getElementById('ROSTER').APOTHECARY.value*50000
	}
	calcTeamValue()
}

function calcTeamValue() {
	teamvalue = 0
	for ( k=0; k<=20; k++) {
		value = parseInt(document.getElementsByName("VALUE[]")[k].value)
		if(isNaN(value) == false && document.getElementsByName("VALUE[]")[k].className != 'injured') {
			teamvalue = teamvalue + value
    	}
    }
	document.getElementById('ROSTER').TEAMVALUE.value = teamvalue
	// color teamvalue red it it is > 1.000.000 gp
	if (teamvalue > 1000000) {
		document.getElementById('ROSTER').TEAMVALUE.style.color = "red"
	}
	else {
		document.getElementById('ROSTER').TEAMVALUE.style.color = "black"
	}
}

function hideLayer(nr) {
	document.getElementById(nr).className = 'element_hidden';
}

function showInjBox(row) {
	if(isPlayerAssigned(row) != false) {
		if(document.getElementById('inj_box').className == 'element_visible') {
			alert(warning[11])
		} else {
			statreductions = new Array(0,0,0)
			injurystring = document.getElementsByName("INJ[]")[row].value
			for (i=0; injurystring.match("-"+MA) != null; i++ ) {
				statreductions[0]++
				injurystring = injurystring.replace("-"+MA,"0")
			}			
			for (i=0; injurystring.match("-"+ST) != null; i++ ) {
				statreductions[1]++
				injurystring = injurystring.replace("-"+ST,"0")
			}			
			for (i=0; injurystring.match("-"+AG) != null; i++ ) {
				statreductions[2]++
				injurystring = injurystring.replace("-"+AG,"0")
			}

			document.getElementById('ROSTER').TEMPMA.value = statreductions[0]
			document.getElementById('ROSTER').TEMPST.value = statreductions[1]
			document.getElementById('ROSTER').TEMPAG.value = statreductions[2]
			output = row + 1
			document.getElementById('ROSTER').TEMP2.value = output
			if(document.getElementsByName("INJ[]")[row].value.charAt(0) == "M") {
				document.getElementById('ROSTER').M.checked = true
			} else {
				document.getElementById('ROSTER').M.checked = false
			}
			document.getElementById('ROSTER').N.value = 0
			for(i=0; i < document.getElementsByName("INJ[]")[row].value.length; i++) {
				if(document.getElementsByName("INJ[]")[row].value.charAt(i) == "N") {
					document.getElementById('ROSTER').N.value++
				}
			}	
			document.getElementById('inj_box').className = 'element_visible';
		}	
	} else {
		alert(warning[3]) 
	}
}

function showJmBox() {
	document.getElementById('jm_box').className = 'element_visible'
	for ( i = 0; i < 16; i++ ) {
		if ( isPlayerAssigned(i) != 2 ) {
			document.getElementById('jm'+i).style.display = "none";
		}
		else {
			document.getElementById('jm'+i).style.display = "block";
		}
	}
}

function showSkillBox(row) {
	if(isPlayerAssigned(row) == true) {
		if(document.getElementById('skill_box').className == 'element_visible') {
			alert(warning[11])
		} 
		else {
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

			document.getElementById('skill_box').className = 'element_visible';
		}
	}
	else { 
		alert(warning[7]) 
	}
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
}

function remskill() {
	row = parseInt(document.getElementById('ROSTER').TEMP1.value - 1)
	position = getFromRoster(row,"POSITION[]")
	
	var x = document.getElementById('ROSTER').SKILLDEST
	selectedskill = x.options[x.selectedIndex].text
	
	if (isStringInArray(selectedskill,skills[position]) == true) {
		alert(warning[12])
	}
	else if (selectedskill == "+MA" || selectedskill == "+ST" || selectedskill == "+AG" || selectedskill == "+AV") {
		alert(warning[13])
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
	document.getElementsByName("SKILLS[]")[row].value = skillString
	value_old = parseInt(document.getElementsByName("VALUE[]")[row].value)
	value_change = parseInt(document.getElementById('ROSTER').TEMP3.value)
	document.getElementsByName("VALUE[]")[row].value = value_old + value_change
	calcTeamValue()
	hideLayer('skill_box')
}

function save() {
	if ( document.getElementsByName("TEAM")[0].value == "" ) {
		alert(warning[14])
	} 
	else {
		document.getElementById('ROSTER').submit()
	}
}

function show(ID) {
	document.getElementById(ID).className = 'element_visible';
}

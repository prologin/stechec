function isPlayerInjured(row) {
	if ( document.getElementsByName("INJURIES[]")[row].value.charAt(0) == "M" ) {
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

function isJourneymanAllowed() {
	if ( countHealthyPlayers() < 12 ) {
		return true
	}
	else {
		return false
	}
}

function isPlayerAssigned(row) {
	pos = parseInt(getFromRoster(row,"POSITION[]"))
	
	if (pos == 0) { 
		return false
	} 
	else {
		if ( pos == positions ) {
			return 2 // roster.js showJmBox token to recognize a journeyman
		}
		return true
	}
}

function isQtyProblem() {

	// enforces the positionlimits
	
	for (i=0; i<=positions; i++) {
	
		// stats[i][7] stores the number of players actually playing that position
		// it has to be resetted

		stats[i][7] = 0
	}
	
	for (i=0; i<16; i++) {
	
		pos = parseInt(getFromRoster(i,"POSITION[]"))
		stats[pos][7]++
		
		if (stats[pos][7] > stats[pos][6]) {

			stats[pos][7]--

			return pos // to use it in the alert
		}
		
	}
	return false
}

function getFromRoster(row,String) {

	// this function will return just anything you ask for ;-)
	return document.getElementsByName(String)[row].value

}

function setRoster(row,String,value) {

	// set a field in the roster to given value
	document.getElementsByName(String)[row].value = value
	
}

function changeRoster(row,String,change) {

	// to increment or decrement a value 
	change = parseInt(change)
	document.getElementsByName(String)[row].value = parseInt(document.getElementsByName(String)[row].value) + change
	
}

function concatRoster(row,String,SubString) {

	// add Substring to the end of String
	// if String is not empty, place a period before the Substring
	
	if (document.getElementsByName(String)[row].value != "") {
		document.getElementsByName(String)[row].value = document.getElementsByName(String)[row].value.concat(",")
	}
	
	document.getElementsByName(String)[row].value = document.getElementsByName(String)[row].value.concat(SubString)
	
}

function findSkill(skillname,position) {

	// the objective is to see whether a skill is a normal, double or forbidden skill for that player
	// it is used when you remove a skill from a player
	
	if (isStringInArray(skillname,skills_g) == true) {
		if (stats[position][8].match("g") != null) { 
			return "normal"
		}
		if (skills[position][9].match("g") != null) {
			return "double"
		}
	}

	if (isStringInArray(skillname,skills_a) == true) {
		if (stats[position][8].match("a") != null) { 
			return "normal"
		}
		if (stats[position][9].match("a") != null) {
			return "double"
		}
	}

	if (isStringInArray(skillname,skills_p) == true) {
		if (stats[position][8].match("p") != null) { 
			return "normal" 
		}
		if (stats[position][9].match("p") != null) {
			return "double"
		}
	}
	
	if (isStringInArray(skillname,skills_s) == true) {
		if (stats[position][8].match("s") != null) { 
			return "normal"
		}
		if (stats[position][9].match("s") != null) {
			return "double"
		}
	}
	
	if (isStringInArray(skillname,skills_m) == true) {
		if (stats[position][8].match("m") != null) { 
			return "normal"
		}
		if (stats[position][9].match("m") != null) {
			return "double"
		}
	}
	
	if (isStringInArray(skillname,skills_e) == true) {
		if (stats[position][8].match("e") != null) { 
			return "normal"
		}
		if (stats[position][9].match("e") != null) {
			return "double"
		}
	}
	
	return "forbidden"
}

function countHealthyPlayers() {

	// first it counts how many healthy players the roster has
	
	healthy_players = 0
	for ( i=0; i<16; i++ ) {
		if ( isPlayerAssigned(i) != false && isPlayerInjured(i) == false ) {
			healthy_players++
		}
	}
	setRoster(0, "HEALTHY", healthy_players)
	
	// then it will return a boolean to say if journeymen are allowed
	
	return healthy_players
}

function setSelects() {
		for ( i=0; i < 16; i++) {
			document.getElementsByName("POSITION[]")[i].selectedIndex = arrpositions[i]
		}
}

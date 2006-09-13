function isPlayerAssigned(row) {

	pos = parseInt(document.getElementsByName("POSITION[]")[row].value)
	
	if (pos == positions) { // the last position-array is the empty one
		return false
	} 
	else {
		if ( pos == positions - 1 ) {
			return 2
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
	
		pos = parseInt(document.getElementsByName("POSITION[]")[i].value)
		stats[pos][7]++
		
		if (stats[pos][7] > stats[pos][6]) {
			alert("You cannot have more than " + stats[pos][6] + " '" + stats[pos][0] + "'.")
			stats[pos][7]--
			
			return true
		}
		
	}
	return false
}

function getFromRoster(row,String) {

	// this function will return just anything you ask for ;-)
	
	result = document.getElementsByName(String)[row].value
	return result

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
					
	
	
	
	
	
	

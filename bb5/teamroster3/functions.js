function eliminateDoubles(arr,arrMaster) {

	// this function will eliminate entries in the first array 
	// that already exist in the second array

	for (i=0; i < arr.length; i++) {
		for (j=0; j < arrMaster.length; j++) {
			if ( arr[i] == arrMaster[j] ) {
				arr.splice(i,1)
			}
		}
	}
	
	// it returns the slave array sorted
	
	arr.sort()
}

function arrayToOptions(arr,Select,value) {
	
	// builds the content of a select list (given with it's name) from the array provided
	// text of each option will be the homologous array element
	// value of each option will be the same
	
	document.getElementsByName(Select)[0].options.length = 0
	
	for (i=0; i < arr.length; i++) {
		document.getElementsByName(Select)[0].options[i] = new Option(arr[i], value)
	}
	
}

function isStringInArray(String,Array) {
	
	// checks if a string is element of an array
	// returns true or false
	
	for (i=0; i < Array.length; i++) {
		if (String == Array[i]) {
			return true
		}
	}
	
	return false
}

function findArrayElement(String,Array) {
	
	// much like isStringInArray but returns the index of the Array Element
	// and not a boolean if found
	
	for (i=0; i < Array.length; i++) {
		if (String == Array[i] ) {
			return i
		}
	}
	
	return -1 
	// that would be a problem so always use isStringInArray first
	
}
		
function deleteArrayElement(String,Array) {

	// deletes an element of an array, element given by it's value
	// this means the given String has to match the element to delete exactly
	
	for (i=0; i < Array.length; i++) {
		if (String == Array[i]) {
			Array.splice(i,1)
		}
	}
	
}

function deleteSelectElement(String,Select) {

	// removes an option from a select list
	
	for (i=0; i < document.getElementsByName(Select)[0].options.length; i++) {
		if(String == document.getElementsByName(Select)[0].options[i].text) {
			document.getElementsByName(Select)[0].remove(i)
			return true
		}
	}
	
	return false
	
}

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

<?php
const	DB_HOST = "mysql.lan.astarc.tk";
const	DB_USER = "astarc";
const	DB_PASS = "#astarc377";
const	DB_NAME = "astarc_iot";

const	TP0L_MOD = 50;

if($_GET['r'] == 'TD'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT VDATA FROM vibrationTD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);

	$TimeDomain = array();

	$HL = str_split(($result->fetch_assoc())['VDATA'], 2);

	for($index = 0; $index < 1024; $index++){
		array_push($TimeDomain, 
			covint(16,
				hexdec(
					'0x'. $HL[2 * $index - 1] 
					. $HL[2 * $index ]
				)
			)
		);
	}


	// echo '<pre>'.print_r($TimeDomain, true).'</pre>';
	echo json_encode($TimeDomain);
	$conn->close();
}

if($_GET['r'] == 'FD'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT VDATA FROM vibrationFD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);

	$FreqDomain = array();

	$HL = str_split(($result->fetch_assoc())['VDATA'], 2);

	for($index = 0; $index < 512; $index++){
		array_push($FreqDomain, 
			// covint(32,
				hexdec(
					'0x'. $HL[( 4 * $index + 3 )]
						. $HL[( 4 * $index + 2 )] 
						. $HL[( 4 * $index + 1 )] 
						. $HL[( 4 * $index + 0 )]
					)
			// )
		);
	}


	// echo '<pre>'.print_r($FreqDomain, true).'</pre>';
	echo json_encode($FreqDomain);
	$conn->close();
}

if($_GET['r'] == 'VIBR'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT VDATA FROM vibrationTD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);
	$TimeDomain = array();
	$HLVIBR = str_split(($result->fetch_assoc())['VDATA'], 2);
	
	$sql = 'SELECT VDATA FROM vibrationFD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);
	$FreqDomain = array();
	$rtnData = array();
	$HLFREQ = str_split(($result->fetch_assoc())['VDATA'], 2);
	
	
	for($index = 0; $index < 512; $index++){
		array_push($FreqDomain, 
			// covint(32,
				hexdec(
					'0x'. $HLFREQ[( 4 * $index + 3 )]
						. $HLFREQ[( 4 * $index + 2 )] 
						. $HLFREQ[( 4 * $index + 1 )] 
						. $HLFREQ[( 4 * $index + 0 )]
					)
			// )
		);
	}
	
	for($index = 1; $index < 1023; $index++){
		array_push($TimeDomain, 
			covint(16,
				hexdec(
					'0x'. $HLVIBR[2 * $index - 1] 
					. $HLVIBR[2 * $index ]
				)
			)
		);
	}
	
	$peakFreq = array_keys($FreqDomain, max($FreqDomain))[0];
	$rtnData['RMS'] = number_format((float)rmsValue($TimeDomain), 1, '.', '').' (mg)';
	$rtnData['Average'] = number_format((float)avgValue($TimeDomain), 1, '.', '').' (mg)';
	$rtnData['Stdev'] = number_format((float)Stand_Deviation($TimeDomain), 1, '.', '');
	// echo '<pre>'.print_r($TimeDomain, true).'</pre>';
	echo json_encode($rtnData);
	$conn->close();
}

if($_GET['r'] == 'TP'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT TIMEMARK,VDATA FROM temperature WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);

	$TempData = str_split(($result->fetch_assoc())['VDATA'], 4);
	$TempJson = array();
	
	$TempCH0 = adc2temp($TempData[0]);
	array_push($TempJson, $TempCH0);
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode($TempJson);
	$conn->close();
}

if($_GET['r'] == 'TP0L'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT TIMEMARK,VDATA FROM temperature WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 30';
	$result = $conn->query($sql);
	$index = 0;
	
	$TempJson = array();
	while($row = $result->fetch_assoc()){
		$TempData = adc2temp(str_split($row['VDATA'],4)[0]);
		$TimeData = explode(' ', $row['TIMEMARK'])[1];
		$TempJson[$TimeData] = $TempData;
	}
	
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode(array_reverse($TempJson));
	$conn->close();
}

if($_GET['r'] == 'EDS'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT TIMEMARK,VDATA FROM EDS WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 60';
	$result = $conn->query($sql);

	$EDSJson = array();
	while($row = $result->fetch_assoc()){
		// $EDSData = hexdec('0x'.str_split($row['VDATA'],4)[0]);
		$EDSData = hexdec('0x'.str_split($row['VDATA'],2)[1].str_split($row['VDATA'],2)[0]);
		$TimeData = explode(' ', $row['TIMEMARK'])[1];
		$EDSJson[$TimeData] = $EDSData/1000;
	}
	
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode(array_reverse($EDSJson));
	$conn->close();
}

if($_GET['r'] == 'EDSsingle'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT TIMEMARK,VDATA FROM EDS WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);

	$EDSJson = array();
	while($row = $result->fetch_assoc()){
		$EDSData = hexdec('0x'.str_split($row['VDATA'],2)[1].str_split($row['VDATA'],2)[0]);
		array_push($EDSJson, $EDSData/1000);
	}
	
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode(array_reverse($EDSJson));
	$conn->close();
}

if($_GET['r'] == 'SENSORMETA'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sqlTD = 'SELECT TIMEMARK FROM vibrationTD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$sqlFD = 'SELECT TIMEMARK FROM vibrationFD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$sqlTemp = 'SELECT TIMEMARK FROM temperature WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$sqlEDS = 'SELECT TIMEMARK FROM EDS WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	
	$rTD = $conn->query($sqlTD);
	$rFD = $conn->query($sqlFD);
	$rTemp = $conn->query($sqlTemp);
	$rEDS = $conn->query($sqlEDS);

	$metaReturn = array();

	while($row = $rTD->fetch_assoc()){
		$metaReturn['vibr'] = array(
			'Last' => $row['TIMEMARK'],
		);
	}
	while($row = $rFD->fetch_assoc()){
		$metaReturn['DSP'] = array(
			'Last' => $row['TIMEMARK'],
		);
	}
	while($row = $rTemp->fetch_assoc()){
		$metaReturn['temp'] = array(
			'Last' => $row['TIMEMARK'],
		);
	}
	while($row = $rEDS->fetch_assoc()){
		$metaReturn['eds'] = array(
			'Last' => $row['TIMEMARK'],
		);
	}
	
	echo json_encode(array_reverse($metaReturn));
	$conn->close();
}

if($_GET['r'] == 'CurrStat'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sqlTD = 'SELECT STATUS FROM vibrationTD ORDER BY TIMEMARK DESC LIMIT 1';
	$sqlFD = 'SELECT STATUS FROM vibrationFD ORDER BY TIMEMARK DESC LIMIT 1';
	$sqlTemp = 'SELECT STATUS FROM temperature ORDER BY TIMEMARK DESC LIMIT 1';
	$sqlEDS = 'SELECT STATUS FROM EDS ORDER BY TIMEMARK DESC LIMIT 1';
	
	$rTD = $conn->query($sqlTD);
	$rFD = $conn->query($sqlFD);
	$rTemp = $conn->query($sqlTemp);
	$rEDS = $conn->query($sqlEDS);

	$metaReturn = array();

	while($row = $rTD->fetch_assoc()){
		$metaReturn['vibr'] = $row['STATUS'];
	}
	while($row = $rFD->fetch_assoc()){
		$metaReturn['DSP'] = $row['STATUS'];
	}
	while($row = $rTemp->fetch_assoc()){
		$metaReturn['temp'] = $row['STATUS'];
	}
	while($row = $rEDS->fetch_assoc()){
		$metaReturn['eds'] = $row['STATUS'];
	}
	
	echo json_encode(array_reverse($metaReturn));
	$conn->close();
}

function adc2temp($adc){
	return hexdec($adc) * 0.061 - 55;
}

function covint($bits, $input){
    if($input < (1 << ($bits-1))){
        return (int)($input);
    } else if ($input >= (1 << $bits)) {
        echo "Error: input is not int"+$bits+" format!!!";
    } else {
        return -(($input ^ ((1 << $bits)-1)) + 1);
    }
}

function Stand_Deviation($arr){
	$num_of_elements = count($arr);
	  
	$variance = 0.0;
	$average = array_sum($arr)/$num_of_elements;
	  
	foreach($arr as $i){
		$variance += pow(($i - $average), 2);
	}
	  
	return (float)sqrt($variance/$num_of_elements);
}

function rmsValue($arr) { 
	$square = 0; 
	$mean = 0.0;
	$root = 0.0; 
	
	$n = sizeof($arr);
	
	// Calculate square. 
	for ($i = 0; $i < $n; $i++){ 
		$square += pow($arr[$i], 2); 
	} 

	// Calculate Mean. 
	$mean = ($square / (float)($n)); 

	// Calculate Root. 
	$root = sqrt($mean); 

	return $root; 
} 

function avgValue($arr) { 
	$sum = 0.0;
	$mean = 0.0;
	
	$n = sizeof($arr);
	for ($i = 0; $i < $n; $i++){ 
		$sum += abs($arr[$i]); 
	} 

	$mean = ($sum / (float)($n)); 
	return $mean; 
} 
?>
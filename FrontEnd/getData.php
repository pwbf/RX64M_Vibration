<?php
const	TD_SHIFT = 2;
const	FD_SHIFT = 4;

const	DB_HOST = "mysql.lan.astarc.tk";
const	DB_USER = "astarc";
const	DB_PASS = "#astarc377";
const	DB_NAME = "astarc_iot";

if($_GET['r'] == 'TD'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT VDATA FROM vibrationTD WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);

	$TimeDomain = array();

	$HL = str_split(($result->fetch_assoc())['VDATA'], 2);

	for($index = 1; $index < 1023; $index++){
		array_push($TimeDomain, 
			covint(16,
				hexdec(
					'0x'. $HL[( TD_SHIFT * $index ) + TD_SHIFT] 
					. $HL[( TD_SHIFT * $index + 1 ) + TD_SHIFT]
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

	for($index = 0; $index < 511; $index++){
		array_push($FreqDomain, 
			// covint(32,
				hexdec(
					'0x'. $HL[( FD_SHIFT * $index + 3 ) + FD_SHIFT] 
						. $HL[( FD_SHIFT * $index + 2 ) + FD_SHIFT] 
						. $HL[( FD_SHIFT * $index + 1 ) + FD_SHIFT] 
						. $HL[( FD_SHIFT * $index ) + FD_SHIFT]
					)
			// )
		);
	}


	echo json_encode($FreqDomain);
	// echo '<pre>'.print_r($FreqDomain, true).'</pre>';
	$conn->close();
}

if($_GET['r'] == 'TP'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT VDATA FROM temperature WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 1';
	$result = $conn->query($sql);

	$TempData = str_split(($result->fetch_assoc())['VDATA'], 4);
	$TempJson = array();
	
	$TempCH0 = adc2temp($TempData[1]);
	$TempCH1 = adc2temp($TempData[1]);
	
	array_push($TempJson, $TempCH0);
	array_push($TempJson, $TempCH1);
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode($TempJson);
	$conn->close();
}

if($_GET['r'] == 'TP0L'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT TIMEMARK,VDATA FROM temperature WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 60';
	$result = $conn->query($sql);

	$TempJson = array();
	while($row = $result->fetch_assoc()){
		$TempData = adc2temp(str_split($row['VDATA'],4)[1]);
		$TimeData = explode(' ', $row['TIMEMARK'])[1];
		$TempJson[$TimeData] = $TempData;
	}
	
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode(array_reverse($TempJson));
	$conn->close();
}

if($_GET['r'] == 'TP1L'){
	$conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
	$sql = 'SELECT TIMEMARK,VDATA FROM temperature WHERE STATUS = 1 ORDER BY TIMEMARK DESC LIMIT 120';
	$result = $conn->query($sql);
	
	$TempJson = array();
	while($row = $result->fetch_assoc()){
		$TempData = adc2temp(str_split($row['VDATA'],4)[2]);
		$TimeData = explode(' ', $row['TIMEMARK'])[1];
		$TempJson[$TimeData] = $TempData;
	}
	
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode($TempJson);
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
		$EDSData = adc2temp(str_split($row['VDATA'],4)[1]);
		$TimeData = explode(' ', $row['TIMEMARK'])[1];
		$EDSJson[$TimeData] = $EDSData;
	}
	
	//echo '<pre>'.print_r($TempData, true).'</pre>';
	echo json_encode(array_reverse($EDSJson));
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
?>
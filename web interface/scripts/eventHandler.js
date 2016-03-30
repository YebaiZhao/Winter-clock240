var dateFromPhoton;

function getTimePhoton(form){ //create the conncetion to the photon and get the time back, set the time in string to the webpage
    console.log("Connecting to the Photon with ID "+form.photon_ID.value);
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webInterface?access_token="+form.photon_Token.value, 
        {arg: "get T"},
        function(data){
            dateFromPhoton=new Date(data.return_value*1000);
            document.getElementById("time_from_photon").innerHTML="Time from the Photon: "+dateFromPhoton.toTimeString();
            console.log(data);
    });		
}
function syncTimePhoton(form){//force the Photon to sync the time and get the new time back to the webpage
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webInterface?access_token="+form.photon_Token.value, 
        {arg: "syncT"},
        function(data){
            dateFromPhoton=new Date(data.return_value*1000);
            document.getElementById("time_from_photon").innerHTML="Current time from the Photon is "+dateFromPhoton.toTimeString()+"(synced)";
            console.log(data);
        }
    );     
}

function turnMinutePhoton(form){
    var minute_modulated="minTr"+form.turn_Minute.value;
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webTuning?access_token="+form.photon_Token.value, 
        {arg:minute_modulated},
        function(data) {
            console.log(data);  
        }
    );
}

function turnHourPhoton(form){
    var hour_modulated="houTr"+form.turn_Hour.value;
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webTuning?access_token="+form.photon_Token.value,
        {arg: hour_modulated},
        function(data) {
            console.log(data);  
        }
    );
}

function moveServoPhoton(form){
    var servo_modulated="ser "+form.servo_Color.value+form.servo_Percentage.value;
    console.log(servo_modulated);
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webTuning?access_token="+form.photon_Token.value,
        {arg: servo_modulated},
        function(data) {
            console.log(data);  
        }
    );
}
var dateFromPhoton;

function getTimePhoton(form){ //create the conncetion to the photon and get the time back, set the time in string to the webpage
    console.log("Connecting to the Photon with ID "+form.photon_ID.value);
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webInterface?access_token="+form.photon_Token.value, 
        {arg: "what is the time, Photon"},
        function(data){
            dateFromPhoton=new Date(data.return_value*1000);
            document.getElementById("time_from_photon").innerHTML="Current time from the Photon is "+dateFromPhoton.toTimeString();
            console.log(data);
    });		
}
function syncTimePhoton(form){//force the Photon to sync the time and get the new time back to the webpage
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webInterface?access_token="+form.photon_Token.value, 
        {arg: "syncTime"},
        function(data){
            dateFromPhoton=new Date(data.return_value*1000);
            document.getElementById("time_from_photon").innerHTML="Current time from the Photon is "+dateFromPhoton.toTimeString()+"(synced)";
            console.log(data);
        }
    );     
}

function turnMinutePhoton(form){
    var minute_modulated=500+Number(form.turn_Minute.value);
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webTuning?access_token="+form.photon_Token.value, 
        {arg:minute_modulated},
        function(data) {
            console.log(data);  
        }
    );
}

function turnHourPhoton(form){
    var hour_modulated=1500+Number(form.turn_Hour.value);
    $.post("https://api.particle.io/v1/devices/"+form.photon_ID.value+"/webTuning?access_token="+form.photon_Token.value,
        {arg: hour_modulated},
        function(data) {
            console.log(data);  
        }
    );
}
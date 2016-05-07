var dateFromPhoton;
var postURI="https://api.particle.io/v1/devices/21001f001647343337363432/webTuning?access_token=aec31db0e35e7f0969dcfbbf7f417210aa391ac5";
var postURI_webInterface="https://api.particle.io/v1/devices/21001f001647343337363432/webInterface?access_token=aec31db0e35e7f0969dcfbbf7f417210aa391ac5";

function getTimePhoton(form){ //create the conncetion to the photon and get the time back, set the time in string to the webpage

    $.post(postURI_webInterface,
        {arg: "get T"},
        function(data){
            dateFromPhoton=new Date(data.return_value*1000);
            document.getElementById("time_from_photon").innerHTML="Time from the Photon: "+dateFromPhoton.toTimeString();
            console.log(data);
        }
    );		
}
function syncTimePhoton(form){//force the Photon to sync the time and get the new time back to the webpage
    $.post(postURI_webInterface, 
        {arg: "syncT"},
        function(data){
            dateFromPhoton=new Date(data.return_value*1000);
            document.getElementById("time_from_photon").innerHTML="Current time from the Photon is "+dateFromPhoton.toTimeString()+"(synced)";
            console.log(data);
        }
    );     
}


function turnMinutePhoton(form, minDiv){
    console.log(minDiv[0].innerText);
    var minute_modulated="minTr"+minDiv[0].innerText;
     $.post(postURI, 
         {arg:minute_modulated},
         function(data) {
             console.log(data);  
         }
     );
}

function turnHourPhoton(form){
    var hour_modulated="houTr12";
    $.post(postURI,
        {arg: hour_modulated},
        function(data) {
            console.log(data);  
        }
    );
}

function moveServoPhoton(form, object){
    console.dir(object[0].value);
    console.dir(object[0].attributes["jiji"].nodeValue);
    // form.servo_Color.value == object[0].attributes["jiji"].nodeValue;
    // form.servo_Percentage.value == object[0].value;

    var servo_modulated="ser "+object[0].attributes["jiji"].nodeValue+object[0].value;
    console.log(servo_modulated);
    $.post(postURI,
        {arg: servo_modulated},
        function(data) {
            console.log(data);  
        }
    );
}

function test(object){
    console.log("testing!!!");
    console.dir(object[0].innerText);

}
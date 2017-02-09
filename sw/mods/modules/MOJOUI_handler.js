window.addEventListener("message",receiveMessage,false);

function receiveMessage(event){
	Console.log(event.message)
}
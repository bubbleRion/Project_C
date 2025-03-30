
const timeHTML = document.getElementById("time")
let time = 5
while(timeHTML+ 0 > 0){
timeHTML.innerHTML = time
setInterval(()=>{
    time -= 1
    timeHTML = time
}, 1000 * 10)
setTimeout(()=>{
    timeHTML = "사용끝"
}, 1000 * 50)
}
const path = require("path")
const express = require('express');
const router = express.Router();

const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);
// info와 템플릿 연결
const info = require("../template/Info.js")
const literal = require("../template/literal.js");
const secretInfo = require("../../private/process/protect.js");

const crypto = require("crypto");
const salt = secretInfo.password.salt

router.get('/', (req, res) => {
  res.sendFile(path.join(__dirname  ,"../html" , "signupPage.html"))
});
 
router.post("/", (req,res)=>{
    let body = req.body;
    
    let saltPw = "" + body.password * salt
    let hashPassword = crypto.createHash(secretInfo.password.hash).update(saltPw).digest(secretInfo.password.digest)
    if(body.id !== "" && body.name !== "" && hashPassword !== "" && body.phone !== ""){
      let newUser = db.query(`insert into ${info.table.user}(userid , dormitoryroom , name , password , phonenumber, email) values("${body.id}", "${body.roomNum}" , "${body.name}","${hashPassword}","${body.phone}", "${body.email}");`, (err, rows)=>{
        if(err){
          console.error(err);
        }
      })
    }
    else{
      console.log("입력 해야함")
    }
    
    res.redirect("/login")
})
module.exports = router;
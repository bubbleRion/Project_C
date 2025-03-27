const path = require("path")
const express = require('express');
const router = express.Router();

const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const secretInfo = require("../../private/process/protect.js");

const crypto = require("crypto");
const info = require("../template/Info.js")
const salt = secretInfo.password.salt

router.get('/', (req, res) => {
    res.sendFile(path.join(__dirname  ,"../html" , "signupPage.html"))
});
 
router.post("/", (req,res)=>{
    let body = req.body;
    
    let saltPw = "" + body.password * salt
    let hashPassword = crypto.createHash(secretInfo.password.hash).update(saltPw).digest(secretInfo.password.digest)
    if(body.id !== "" && body.name !== "" && hashPassword !== "" && body.phone !== ""){
      let newUser = db.query(`insert into ${info.table.user}(id,name,address,phone,password, email) values("${body.id}","${body.name}","${body.address}","${body.phone}","${hashPassword}" , "${body.email}");`, (err, rows)=>{
        if(err){
          console.error(err);
        }
      })
    }
    else{
      console.log("입력 해야함")
    }
    
      res.redirect("/")
})
module.exports = router;
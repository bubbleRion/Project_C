const express = require('express')
const router = express.Router();
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const secretInfo = require("../../private/process/protect.js");
const info = require("../template/Info.js")
const path = require("path");
const literal = require("../template/literal.js");

router.post("/" , (req , res)=>{
    db.query(`update ${info.table.wash} set is_working = 1 , dormitoryroom = "${domi}" , phonenumber = "${phone}" where id = 3};` , (err)=>{
        console.log(domi)
        if(err) console.error(err)
    })
    res.send(literal.washDetail(idx, domi , phone))
    
})
module.exports = router
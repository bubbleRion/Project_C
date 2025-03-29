const express = require('express')
const router = express.Router();
const conn = require("../../private/process/db.js")
const mysql = require("mysql")
const db = mysql.createConnection(conn);

const secretInfo = require("../../private/process/protect.js");
const info = require("../template/Info.js")
const path = require("path");
const literal = require("../template/literal.js");

router.get("/" , (req , res)=>{
    
    
    
    let domi , phone
    let sql1 = db.query(`select dormitoryroom , phonenumber from ${info.table.user}`)
    db.query(sql1, (err , row)=>{
        if(err) console.error(err)
        row.foreach((item , idx)=>{
            if(idx == 0){
                domi = item
                console.log("bye")
            }
            else{
                phone = item
            }
        })
        db.query(`update ${info.table.wash} set is_working = 1 , dormitoryroom = "${domi}" , phonenumber = "${phone}" where id = 4};` , (err)=>{
            console.log(domi)
            if(err) console.log(err)
            res.send(literal.washDetail(idx, domi , phone))
        })
    })
    
    

    
    
    
})
module.exports = router
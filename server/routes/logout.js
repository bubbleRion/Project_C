const express = require('express');
const router = express.Router();

router.get("/", (req, res)=>{
    const userId = req.session.id
    if(userId){
        req.session.userID = null
    }
    res.redirect("/")


})
module.exports = router;

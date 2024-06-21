const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>HMS App</title>
      <style>
      * {
    padding: 0px;
    margin: 0px;
    box-sizing: border-box;

}

body {
    min-height: 100vh;
    background: linear-gradient(180deg, #ffe700, #ff7b00, #ffe700,
            #1f1f25, #1f1f25, #1f1f25);
    background-color: #1f1f25;
    justify-content: flex-start;
}

header {
    margin: 30px;
    padding: 0px 20px;
    align-items: center;
    width: 100%;
}

header h1 {
    /* color: #02191d; */
    color: #1f1f25;
}

.even {
    justify-content: space-evenly;
}

h1 {
    font-size: xx-large;
    display: block;
}

h2,
.btn {
    font-size: medium;
    font-weight: bold;
}

h2 {
    margin-bottom: 10px;
}

p,
label,
h1,
h2,
h3,
.footer,
.settings_select,
.settings_btn,
select {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    color: rgba(28, 46, 73, 0.918);
}

label,
.settings_select {
    font-size: medium;
    display: block;
}

.dflex {
    display: flex;
}

.jcol {
    flex-direction: column;
    -ms-flex-direction: column;
}

.jcol-reverse {
    flex-direction: column-reverse;
    -ms-flex-direction: column-reverse;
}

.wrap {
    flex-wrap: wrap;
    -ms-flex-wrap: wrap;
}

.nowrap {
    flex-wrap: nowrap;
    -ms-flex-wrap: nowrap;
}

.jrow {
    flex-direction: row;
    -ms-flex-direction: row;
}

.sb {
    justify-content: space-between;
}

.flexend {
    justify-content: flex-end;
}

.end {
    align-items: flex-end;
    -ms-flex-item-align: flex-end;
}

.center {
    align-items: center;
    -ms-flex-item-align: center;
}

.jcenter {
    justify-content: center;
}

.hidden {
    display: none;
}

input,
.btn,
select {
    font-size: medium;
    padding: 10px;
    margin-bottom: 10px;
    margin-top: 10px;
}

input {
    width: 100%;
    border-radius: 5px;
    border: 1px solid #1f1f25;
}

.btn {
    /* background: linear-gradient(25deg, #1f1f1f, #1f1f25); */
    background-color: #1f1f25;
    transition: background-color 0.3s ease-in-out;
    border: 1px solid #ffbb00;
    border-radius: 50px;
    width: 100px;
    color: white;
}

.settings_select {
    border: 1px solid;
    background-color: transparent !important;
    transition: background-color 0.3s ease-in-out;
    text-align: left;
}

.settings_btn {
    font-size: medium;
    width: 70px;
}

.bold {
    font-weight: bold;
}

.mt-10 {
    margin-top: 10px;
}

.mb-10 {
    margin-bottom: 10px !important;
}

.mb-20 {
    margin-bottom: 20px !important;
}

.settings_select:hover {
    background-color: #e2dfdf !important;
}

.btn:hover {
    background-color: #31313a;
}

.UI,
.sel_UI {
    margin: 0px auto 0px auto;
    width: 100%;
}

.UI, .measure{
    background-color: #f5f4ff80;
    box-shadow: 0px 1px 2px 0px #1f1f25;
    border-radius: 8px;
}

.UI {
    max-width: 430px;
    width:100%;
    padding: 20px;
}

.measure{
 font-size:smaller !important;
 align-self: center;
}

.sel_UI {
    padding: 10px;
    border-radius: 15px;
}

.main-container {
    display: flex;
    justify-content: space-between;
    flex-direction: column;
}

.userUI {
    padding: 20px 20px 10px 20px;
    margin: 0px auto;
    width: 100%;
    /* max-width: 600px; */
    background: linear-gradient(25deg, #ccccccdd, #e4e4e7dd, #ffffffdd, #e4e4e7dd, #ccccccdd);
    border-radius: 30px 30px 0px 0px;
}


hr {
    height: 1px;
    max-width: 500px;
    width: 90%;
    margin: 20px auto;
}

footer {
    margin: 0px auto;
    padding: 30px;
}

footer p {
    text-align: center;
    color: whitesmoke !important;
}

a {
    text-decoration: none;
    color: #ff7b00;
}

.buttons {
    height: 70px;
    justify-content: space-between;
}

/* For screen width higher than 600px */
@media only screen and (min-width: 600px) {
    .main-container {
        display: grid;
        grid-template-columns: auto auto;
        /* grid-column: 1 / span 2; */
        gap: 30px;
    }

    .mb-20 {
        margin-bottom: 0px;
    }

    .sel_UI {
        max-width: 715px;
    }

    .buttons{
        justify-content: space-evenly;
    }
}

@media only screen and (min-width: 800px) {
    .main-container{
        grid-template-columns: auto auto auto;
    }

    .sel_UI{
        max-width:950px;
    }
}

@media only screen and (min-width: 1200px) {
    .main-container{
        grid-template-columns: auto auto auto auto;
    }

    .sel_UI{
        max-width:100vw;
    }
}
    </style>
</head>

<body class="dflex jcol center even">
    <header class="dflex sb wrap">
        <h1>Welcome <span id="username_info">Back</span>!</h1>

        <select class="measure" id="int">
            <option>Set Measurement Interval</option>
            <option>1</option>
            <option>10</option>
            <option>20</option>
            <option>30</option>
            <option>40</option>
            <option>50</option>
            <option>60</option>
        </select>
    </header>
    <section class="userUI">
        <section class="main-container sel_UI">
            <div class="dflex jcol UI mb-20 user-info">
                <h2 class="settings_select UI">User Information</h2>
                <div class="mt-10">
                    <label for="fullname">Full Name:</label>
                    <input id="fullname" type="text" placeholder="Enter Name" required>
                    <label for="enterUser">Login Username:</label>
                    <input id="enterUser" type="text" placeholder="Enter Admin Password">
                    <label for="enterPass">Login Password:</label>
                    <input id="enterPass" type="password" placeholder="Enter Admin Password">
                </div>
            </div>

            <div class="dflex jcol UI mb-20 sim">
                <h2 class="settings_select UI">SIM and SOS Number Setting</h2>

                <div class="dflex sb jcol mt-10">
                    <label for="carrierSIM">Select Carrier Network</label>
                    <select id="carrierSIM">
                        <option>MTN</option>
                        <option>GLO</option>
                        <option>AIRTEL</option>
                        <option>9MOBILE</option>
                    </select>

                    <label for="sosNumber2">Enter SOS Number</label>
                    <input type="tel" id="sosNumber1" placeholder="SOS Number 1" required>
                    <label for="sosNumber2">Enter SOS Number</label>
                    <input type="tel" id="sosNumber2" placeholder="SOS Number 2">

                </div>
            </div>

            <div class="dflex jcol UI mb-20 network">
                <h2 class="settings_select UI">Change Network Connection Setting</h2>
                <div class="mt-10">
                    <label for="ssid">SSID:</label>
                    <input id="ssid" type="text" placeholder="Enter WiFi SSID">
                    <label for="pass">Password:</label>
                    <input id="pass" type="password" placeholder="Enter WiFi Password">
                </div>
            </div>

            <div class="dflex jcol UI mb-20">
                <h2 class="settings_select UI">Clinical Blood Pressure Values</h2>
                <div class="mt-10">
                    <label for="sys">Systolic:</label>
                    <input id="sys" type="text" placeholder="Enter Systolic Value">
                    <label for="dia">Diastolic:</label>
                    <input id="dia" type="text" placeholder="Enter Diastolic Value">
                </div>
            </div>

        </section>
        <div class="dflex mt-10 buttons">
            <button class="btn" id="admin_settings">Save</button>
            <button class="btn" id="logout">Complete</button>
        </div>
    </section>

    <footer>
        <p>Designed by <a href="https://g.page/r/CfQvFv81BsfaEAI/">Newness Electronics</a><span>
                &copy</span>2023</p>
    </footer>

    <script>
      window.addEventListener("DOMContentLoaded", () => {
          async function getData(path) {
              const response = await fetch(`${path}`);
              const textData = await response.text();
              // console.log(jsonData);
              return textData;
          }

          const parseNetwork = {
              MTN: 'web.gprs.mtnnigeria.net',
              GLO: 'gloflat',
              AIRTEL: 'internet.ng.airtel.com',
              '9MOBILE': '9mobile'
          }

          fullname.addEventListener('input', () => {
              enterUser.value = fullname.value.toLowerCase().replaceAll(' ', '')
          })

          admin_settings.addEventListener('click', () => {
              let addr = `/admin?`
              fullname.value ? addr += `fullname=${fullname.value}&` : addr += '';
              enterUser.value ? addr += `username=${enterUser.value}&` : addr += '';
              enterPass.value ? addr += `password=${enterPass.value}&` : addr += '';
              int.value == 'Set Measurement Interval' ? addr += '' : addr += `interval=${int.value}&`;
              ssid.value ? addr += `ssid=${ssid.value}&` : addr += '';
              pass.value ? addr += `pass=${pass.value}&` : addr += '';
              sosNumber1.value ? addr += `sos1=${sosNumber1.value}&` : addr += '';
              sosNumber2.value ? addr += `sos2=${sosNumber2.value}&` : addr += '';
              sys.value ? addr += `sbp=${sys.value}&` : addr += '';
              dia.value ? addr += `dbp=${dia.value}&` : addr += '';
              addr += `net=${parseNetwork[carrierSIM.value]}`

              const finaladdr = addr.replaceAll('#','hash')
              getData(finaladdr).then((r) => {
                  console.log(r);
                  r.startsWith('<') ? alert('Error! Try again') : alert('Saved successfully')
              })
          })

          window.addEventListener("load", () => {
              getData('/load').then((r) => {
                console.log(r)
                  r.startsWith('<') ? username_info.innerText = "Back" : username_info.innerText = r.slice(r.lastIndexOf(' '));
              })
          })


          logout.addEventListener('click', () => {
              let addr = `/logout`
              confirm("Are you sure?") ? getData(addr).then((r) => {
                  console.log(r);
                  setTimeout(window.open("/logged-out", "_self"), 1000);
              }) : null
          })
      });
    </script>
</body>
</html>
)rawliteral";

// HTML code for logout page
const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>HRM:Logout</title>
</head>
<style>
html {font-family: Helvetica; display: inline-block; text-align: center;}
</style>    
<body>
<h2 style="height:40px;width:100%;background-color:#333;"></h2>
<h2>Health Monitoring System</h2>
  <p>Logged out</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";
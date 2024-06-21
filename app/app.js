window.addEventListener("DOMContentLoaded", () => {
    async function getData(path) {
        const response = await fetch(`${path}`);
        const textData = await response.text();
        // console.log(jsonData);
        return textData;
    }

    const removeHash = (e) => {
        
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
        sys.value ? addr += `sys=${sys.value}` : addr += '';
        dia.value ? addr += `dia=${dia.value}` : addr += '';
        addr += `net=${parseNetwork[carrierSIM.value]}`

        const finaladdr = addr.replaceAll('#','hash')
        getData(finaladdr).then((r) => {
            console.log(r);
            r.startsWith('<') ? alert('Error! Try again') : alert('Saved successfully')
        })
    })

    window.addEventListener("load", () => {
        getData('/load').then((r) => {
            console.log(r);
            r = "Gabriel Ayoola Emmanuel"
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

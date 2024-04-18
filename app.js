const API_KEY = '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';
const submitButton = document.querySelector('#submit')
const outputElement = document.querySelector('#output')
const inputElement = document.querySelector('#input')
const historyElement = document.querySelector('.history')


async function getMessage(){
    const options = {
        method: "POST",
        headers: {
            Authorization: `Bearer ${API_KEY}`,
            "Content-Type": "application/json",
        },
        body: JSON.stringify({
            model: "gpt-3.5-turbo", // Make sure this is the correct model identifier
            messages: [{
                role: "user",
                content: inputElement.value
            }],
        })
    }
    try{
        const response = await fetch("https://api.openai.com/v1/chat/completions", options)
        const data = await response.json()
        outputElement.textContent = data.choices[0].message.content
        if(data.choices[0].message.content){
            const pElement = document.createElement('p')
            pElement.textContent = inputElement.value
            historyElement.append(pElement)

        }
    }
    catch (error){
        console.error(error)

    }
}

submitButton.addEventListener('click', getMessage)

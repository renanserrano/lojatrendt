# lojatrendt
Falta falar da grade, bolha, código do ESP, arduino, telegram e herokuapp

- Vai no shopify e cria um private app API: https://yourstorename.myshopify.com/admin/apps/private/new
- Depois vai no https://app.shopify.com/services/partners/api_clients/ e inclui:
- Callback url: http://t.me/trendtbot e o API informado pelo telegram
- Credentias set: Show code e coloca no Herokuapp

#Alimentação
- Utilizar jogo de pilhas recarregáveis com no máximo 6V. o positivo vai para a protoboard e de lá vai pra porta VIN
#Servos
- Fio vermelho (positivo) ao invés de ligar no 5V. do arduino, primeiro liga todos numa protoboard e a saida da placa vai pra porta VIN
- Fio Preto (ground) ligar na protoboard em linha com arduino e alimentação, criando assim um ground comun entre todos
#Arduino UNO
- Jumper da porta vin para a placa ligada em linha com a alimentação e os servos
#ESP8266 NodeMcu
- Precisa de uma alimentação paralela que pode ser um outro jogo de pilhas de 6V mas esse ligado direto a ele, sem passar pela protoboard

(onsequel.com) faz bot rapidos mas não foi usado pra esse projeto, o bom dele é que conecta com FB messenger mas precisa de raspberri

#Herokuapp
- Sobe a pasta Middleware no deploy heroku php composer

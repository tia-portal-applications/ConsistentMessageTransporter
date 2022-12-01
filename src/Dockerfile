FROM node:10.19

WORKDIR /app

COPY package.json .

RUN npm install

EXPOSE 23456
COPY . .

CMD ["node", "Server.js","--southboundservice=unified",  "--log=verbose"] 

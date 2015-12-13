Installation scripts:

npm install
npm install -g particle-cli

This two have been added to package.json. I don't think they are needed separately.
npm install googleapis --save
npm install google-auth-library --save

Google Calendar API instructions: https://developers.google.com/google-apps/calendar/quickstart/nodejs

Each developer will need to:
update the .env file with the appropriate SPARK_TOKEN
store a clien_secret.json file in server/ for the Google Calendar API

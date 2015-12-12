import google from 'googleapis'
import loadSecret from './authorize'

function postEvents(auth) {
  // Refer to the Node.js quickstart on how to setup the environment:
  // https://developers.google.com/google-apps/calendar/quickstart/node
  // Change the scope to 'https://www.googleapis.com/auth/calendar' and delete any
  // stored credentials.
  var calendar = google.calendar('v3');
  var event = {
    'summary': 'Google I/O 2015',
    'location': '800 Howard St., San Francisco, CA 94103',
    'description': 'A chance to hear more about Google\'s developer products.',
    'start': {
      'dateTime': '2015-12-15T09:00:00-07:00',
      'timeZone': 'America/Los_Angeles',
    },
    'end': {
      'dateTime': '2015-12-15T17:00:00-07:00',
      'timeZone': 'America/Los_Angeles',
    },
    'recurrence': [
      'RRULE:FREQ=DAILY;COUNT=2'
    ],
    'attendees': [
      {'email': 'lpage@example.com'},
      {'email': 'sbrin@example.com'},
    ],
    'reminders': {
      'useDefault': false,
      'overrides': [
        {'method': 'email', 'minutes': 24 * 60},
        {'method': 'popup', 'minutes': 10},
      ],
    },
  };

  calendar.events.insert({
    auth: auth,
    calendarId: 'primary',
    resource: event,
  }, function(err, event) {
    if (err) {
      console.log('There was an error contacting the Calendar service: ' + err);
      return;
    }
    console.log('Event created: %s', event.htmlLink);
  });
}

function startEvent(googleCalendarId, { duration }) {
  console.log('starting event for ', googleCalendarId, duration)

  return new Promise((resolve, reject) => {
    // const resp = loadSecret(postEvents)
    // resolve(resp)
    console.warn('resolving')
    resolve('12345')
  })
}

function endEvent(googleCalendarId, { eventId }) {
  console.log('ending event for: ', googleCalendarId, eventId)

  loadSecret(postEvents)
}

export {
  startEvent,
  endEvent
}

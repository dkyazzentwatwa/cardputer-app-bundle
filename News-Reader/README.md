# News Reader

Minimal Cardputer news search reader for a home Wi-Fi connection.

## Configure

For local source builds you may copy `src/secrets.h.template` to
`src/secrets.h`, then fill in:

- `NEWS_WIFI_SSID`
- `NEWS_WIFI_PASSWORD`
- `GUARDIAN_API_KEY`

For Cypher OS release builds, put the same values on the SD card at
`/news-reader/config.txt`:

```text
NEWS_WIFI_SSID=your-home-wifi
NEWS_WIFI_PASSWORD=your-wifi-password
GUARDIAN_API_KEY=your-guardian-api-key
```

Guardian developer keys are free for non-commercial use:

https://open-platform.theguardian.com/access

## Use

- Type a search query on the first screen.
- Press `Enter` to fetch matching articles.
- Use `Up` / `Down` to move through individual results.
- Press `Enter` to read the selected item.
- Use `Up` / `Down` while reading to scroll.
- Press `Backspace` to go back.
- Press `s` on the results screen to enter a new search.
- Press `Fn+Del` to return to Cypher OS when launched from the Cypher OS catalog.

## Compile

```sh
arduino-cli compile --profile cardputer News-Reader
```

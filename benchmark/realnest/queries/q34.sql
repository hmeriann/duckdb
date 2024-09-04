SELECT 
  list_aggregate(tau_pt, 'sumkahan'),
  list_aggregate(tau_eta, 'sumkahan'),
  list_aggregate(jet_pt, 'sumkahan'),
  list_aggregate(jet_eta, 'sumkahan'),
  list_aggregate(muon_pt, 'sumkahan'),
  list_aggregate(muon_eta, 'sumkahan'),
  list_aggregate(el_pt, 'sumkahan'),
  list_aggregate(el_eta, 'sumkahan'),
  list_aggregate(ph_pt, 'sumkahan'),
  list_aggregate(ph_eta, 'sumkahan')  
FROM singleMu;
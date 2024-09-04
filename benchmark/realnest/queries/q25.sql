SELECT 
  list_aggregate(tau_pt, 'mode'),
  list_aggregate(tau_eta, 'mode'),
  list_aggregate(jet_pt, 'mode'),
  list_aggregate(jet_eta, 'mode'),
  list_aggregate(muon_pt, 'mode'),
  list_aggregate(muon_eta, 'mode'),
  list_aggregate(el_pt, 'mode'),
  list_aggregate(el_eta, 'mode'),
  list_aggregate(ph_pt, 'mode'),
  list_aggregate(ph_eta, 'mode')  
FROM singleMu;